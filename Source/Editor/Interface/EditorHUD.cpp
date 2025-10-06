// ReSharper disable CppDFANullDereference
#include "EditorHUD.h"

#include "Actors/ActorManager.h"
#include "Application/Application.h"
#include "Engine/Game.h"
#include "Engine/MapManager.h"
#include "Engine/World.h"
#include "Interface/ButtonGroup.h"
#include "Interface/EditText.h"
#include "Interface/Menu.h"
#include "Interface/Panel.h"
#include "Interface/ScrollArea.h"
#include "Interface/Spinner.h"

#include "EditorGame.h"

PEditorGame* GEditorGame = nullptr;

static PWidget*							 MainPanel = nullptr;
static PPanel*							 SelectPanel = nullptr;
static PPanel*							 TilePanel = nullptr;
static PPanel*							 ActorPanel = nullptr;
static PWidget*							 ActorParamView = nullptr;
static std::map<std::string, PGridView*> TilesetViews;
static PButtonGroup*					 TilesetViewButtonGroup = nullptr;
static PButtonGroup*					 ActorViewButtonGroup = nullptr;

std::vector<SActorItem>							 gPlaceableActors{};
std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{ "JSON", "JSON" },
};

PEditorHUD::PEditorHUD()
{
	GEditorGame = GEngine->GetGameAs<PEditorGame>();

	ActorManager::LoadActorDefs();
	for (auto& [K, V] : ActorManager::GetActorDefs().items())
	{
		gPlaceableActors.push_back(SActorItem(K, V));
	}

	SetupInterface();

	// Default edit menu
	OnSelectButtonClicked();
	GEditorGame->SelectionChanged.AddRaw(this, &PEditorHUD::OnSelectionChange);
}

void PEditorHUD::SetupInterface()
{
	// Vertical layout to force the file menu to the top
	mLayoutMode = LM_Vertical;

	// File Menu
	auto MenuBar = ConstructWidget<PMenuBar>();
	MenuBar->AddMenu("File",
					 {
						 { "New", this, &PEditorHUD::OnNewButtonClicked },
						 { "Load", this, &PEditorHUD::OnLoadButtonClicked },
						 { "Save", this, &PEditorHUD::OnSaveButtonClicked },
						 {},
						 { "Exit", this, &PEditorHUD::OnExitButtonClicked },
	 });
	MenuBar->AddMenu("Edit",
					 {
						 { "Create", this, &PEditorHUD::OnCreateButtonClicked },
						 {},
						 { "Select", this, &PEditorHUD::OnSelectButtonClicked },
						 { "Tiles", this, &PEditorHUD::OnTilesButtonClicked },
						 { "Actors", this, &PEditorHUD::OnActorsButtonClicked },
	 });

	AddChild(MenuBar);

	// Main panel
	MainPanel = GWorld->ConstructWidget<PWidget>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(340);
	MainPanel->SetVisible(false);

	// Select

	SelectPanel = GWorld->ConstructWidget<PPanel>();
	SelectPanel->SetResizeModeH(RM_Fit);
	SelectPanel->SetLayoutMode(LM_Vertical);
	MainPanel->AddChild(SelectPanel);

	// Tiles

	TilePanel = GWorld->ConstructWidget<PPanel>();
	TilePanel->SetLayoutMode(LM_Vertical);
	auto ScrollArea = GWorld->ConstructWidget<PScrollArea>();
	TilePanel->AddChild(ScrollArea);
	// Create a button group for all tiles across all tilesets
	TilesetViewButtonGroup = GWorld->ConstructWidget<PButtonGroup>();
	// Construct each widget for each tile in each tileset
	for (const auto Tileset : TilesetManager::GetTilesets())
	{
		auto TilesetView = ConstructTilesetView(Tileset);
		ScrollArea->AddChild(TilesetView);
		TilesetViews[Tileset->Name] = TilesetView;
	}

	// Actors
	ActorViewButtonGroup = ConstructWidget<PButtonGroup>();
	ActorPanel = GWorld->ConstructWidget<PPanel>();
	ActorPanel->SetLayoutMode(LM_Vertical);
	auto ActorView = ConstructActorView();
	ActorPanel->AddChild(ActorView);

	// Main panel

	AddChild(MainPanel);
}

PGridView* PEditorHUD::ConstructTilesetView(STileset* Tileset)
{
	const int ItemSize = 20;
	const int ViewWidth = Tileset->Width * ItemSize;
	const int ViewHeight = Tileset->Height * ItemSize;

	PGridView* GridView = GWorld->ConstructWidget<PGridView>();
	GridView->SetGridCount(Tileset->Width);
	GridView->SetFixedWidth(ViewWidth);
	GridView->SetResizeMode(RM_Fixed, RM_Fixed);
	GridView->SetFixedHeight(ViewHeight);

	auto TilesetTexture = Tileset->Texture;

	for (auto& Item : Tileset->Tiles)
	{
		// Create the button item
		auto GridItem = GridView->AddItem<PButton>(TilesetTexture);
		auto Button = GridItem->GetWidget<PButton>();
		Button->Padding = { 0 };
		Button->SetResizeMode(RM_Fixed, RM_Fixed);
		Button->SetFixedSize(ItemSize);
		Button->SetCheckable(true);
		Button->SetCustomData(&Item);
		Button->SetUseSourceRect(true);
		Button->SetSourceRect(Item.GetSourceRect());
		Button->Checked.AddRaw(this, &PEditorHUD::OnTilesetButtonChecked);

		TilesetViewButtonGroup->AddButton(Button);
	}

	return GridView;
}

PGridView* PEditorHUD::ConstructActorView()
{
	if (!GWorld || !GEditorGame)
	{
		return nullptr;
	}
	PGridView* GridView = GWorld->ConstructWidget<PGridView>();
	GridView->Padding = { 5 };
	GridView->SetGridCount(1);

	for (auto& ActorItem : gPlaceableActors)
	{
		const int ItemSize = 40;
		// Create the button item
		auto GridItem = GridView->AddItem<PButton>(ActorItem.Name);
		auto Button = GridItem->GetWidget<PButton>();
		Button->SetResizeMode(RM_Grow, RM_Fixed);
		Button->SetFixedHeight(ItemSize);
		Button->SetCheckable(true);
		Button->SetCustomData(&ActorItem);
		Button->Checked.AddRaw(this, &PEditorHUD::OnActorButtonChecked);

		ActorViewButtonGroup->AddButton(Button);
	}

	return GridView;
}

PWidget* PEditorHUD::ConstructParamRow(PParameter* Param)
{
	// Row
	auto ParamRow = ConstructWidget<PWidget>();
	ParamRow->SetResizeModeH(RM_Fixed);
	ParamRow->SetFixedHeight(DEFAULT_WIDGET_HEIGHT);

	// Left-hand label
	auto ParamLabel = ConstructWidget<PText>(std::format("{}: ", Param->GetName()));
	ParamLabel->SetAlignment(AL_Left);
	ParamLabel->SetResizeModeW(RM_Fixed);
	ParamLabel->SetFixedWidth(75);
	ParamRow->AddChild(ParamLabel);

	// Right-hand widget
	switch (Param->GetType())
	{
		case PT_String:
			{
				auto EditText = ConstructWidget<PEditText>();
				EditText->Bind(Param);
				ParamRow->AddChild(EditText);
				break;
			}
		case PT_FVector3:
			{
				auto MultiSpinner = ConstructWidget<PWidget>();

				auto SpinnerX = ConstructWidget<PSpinner>();
				SpinnerX->SetStep(50);
				SpinnerX->SetColor(PColor::Red);
				SpinnerX->Bind(Param, 0);
				MultiSpinner->AddChild(SpinnerX);

				auto SpinnerY = ConstructWidget<PSpinner>();
				SpinnerY->SetStep(50);
				SpinnerY->SetColor(PColor::Green);
				SpinnerY->Bind(Param, 1);
				MultiSpinner->AddChild(SpinnerY);

				auto SpinnerZ = ConstructWidget<PSpinner>();
				SpinnerZ->SetStep(50);
				SpinnerZ->SetColor(PColor::Blue);
				SpinnerZ->Bind(Param, 2);
				MultiSpinner->AddChild(SpinnerZ);

				ParamRow->AddChild(MultiSpinner);
				break;
			}
		default:
			break;
	}

	// Add row
	return ParamRow;
}

PWidget* PEditorHUD::ConstructParamView(IParamBlock* ParamBlock, const std::string& Title)
{
	auto ParamBlockView = ConstructWidget<PBox>();
	ParamBlockView->Padding = { 5 };
	ParamBlockView->SetLayoutMode(LM_Vertical);
	ParamBlockView->SetResizeModeH(RM_Fit);

	for (auto [Name, Param] : ParamBlock->GetAllParameters())
	{
		ParamBlockView->AddChild(ConstructParamRow(Param));
	}

	return ParamBlockView;
}

void PEditorHUD::OnSizeXChanged(float Value)
{
	mNewMapSizeX = Value;
}

void PEditorHUD::OnSizeYChanged(float Value)
{
	mNewMapSizeY = Value;
}

void PEditorHUD::OnNewButtonClicked()
{
	GWorld->DestroyAllActors();
	GMapManager->ClearMaps();
}

void PEditorHUD::OnCreateButtonClicked()
{
	auto CurrentTileset = GEditorGame->GetCurrentTileset();
	int	 TileCountX = mNewMapSizeX * 2;
	int	 TileCountY = mNewMapSizeY * 2;
	LogDebug("Creating new map: [{}, {}]", TileCountX, TileCountY);

	JSON JsonData = {
		{  "MapName",	 "NewMap" },
		{ "Position",	  { 0, 0 } },
		{	  "SizeX", TileCountX },
		{	  "SizeY", TileCountY },
	};
	for (int X = 0; X < TileCountX; ++X)
	{
		for (int Y = 0; Y < TileCountY; ++Y)
		{
			auto Tileset = CurrentTileset ? CurrentTileset->Name : TILESET_1;
			JsonData["Tiles"].push_back({
				{	  "Index",	   0 },
				{ "Tileset", Tileset }
			   });
		}
	}

	GMapManager->ConstructMap(JsonData);
}

void PEditorHUD::OnSaveButtonClicked()
{
	JSON Json;

	for (const auto Actor : GWorld->GetActors())
	{
		if (auto GameMap = dynamic_cast<PGameMap*>(Actor))
		{
			Json = GameMap->Serialize();
			break;
		}
	}

	std::string FileName;
	if (Files::GetSaveFileName(&FileName, gDefaultFilters))
	{
		LogDebug("Saving to file: {}", FileName);
		Files::WriteFile(FileName, Json.dump(4));
	}
}

void PEditorHUD::OnLoadButtonClicked()
{
	std::string FileName;

	if (!Files::GetOpenFileName(&FileName, gDefaultFilters))
	{
		return;
	}

	GMapManager->LoadMap(FileName, true);
}

void PEditorHUD::OnSelectButtonClicked()
{
	if (IsInputContext(IC_Select))
	{
		return;
	}
	SetInputContext(IC_Select);
	MainPanel->AddChild(SelectPanel);
	MainPanel->RemoveChild(TilePanel);
	MainPanel->RemoveChild(ActorPanel);
}

void PEditorHUD::OnTilesButtonClicked()
{
	if (IsInputContext(IC_Tile))
	{
		return;
	}
	SetInputContext(IC_Tile);
	MainPanel->RemoveChild(SelectPanel);
	MainPanel->AddChild(TilePanel);
	MainPanel->RemoveChild(ActorPanel);
}

void PEditorHUD::OnActorsButtonClicked()
{
	if (IsInputContext(IC_Actor))
	{
		return;
	}
	SetInputContext(IC_Actor);
	MainPanel->RemoveChild(SelectPanel);
	MainPanel->RemoveChild(TilePanel);
	MainPanel->AddChild(ActorPanel);
}

void PEditorHUD::OnExitButtonClicked()
{
	GEditorGame->End();
}

void PEditorHUD::OnSelectionChange(PActor* Actor)
{
	SelectPanel->RemoveAllChildren();
	if (!Actor && ActorParamView != nullptr)
	{
		GWorld->DestroyWidget(ActorParamView);
		return;
	}

	// Destroy this widget when the actor is destroyed
	Actor->Destroyed.AddRaw(this, &PEditorHUD::OnActorDestroyed);

	ActorParamView = ConstructParamView(dynamic_cast<IParamBlock*>(Actor), Actor->GetClassName());
	SelectPanel->AddChild(ActorParamView);

	if (Actor->GetComponentsCount() > 0)
	{
		auto CompGroup = ConstructWidget<PBox>();
		CompGroup->Padding = { 5 };
		CompGroup->SetLayoutMode(LM_Vertical);
		CompGroup->SetResizeModeH(RM_Fit);
		CompGroup->SetResizeModeW(RM_Grow);

		for (auto Comp : Actor->GetComponents())
		{
			auto CompParamView = ConstructParamView(dynamic_cast<IParamBlock*>(Comp), Comp->GetClassName());
			CompGroup->AddChild(CompParamView);
		}

		SelectPanel->AddChild(CompGroup);
	}
}

void PEditorHUD::OnTilesetButtonChecked(bool State)
{
	auto Sender = GetSender<PButton>();
	auto Item = Sender->GetCustomData<STileItem>();

	GEditorGame->SetCurrentTileset(Item->Tileset);
	GEditorGame->SetCurrentTilesetItem(State ? Item : nullptr);
}

void PEditorHUD::OnActorButtonChecked(bool State)
{
	auto Sender = GetSender<PButton>();
	auto Item = Sender->GetCustomData<SActorItem>();
	GEditorGame->SetCurrentActorItem(State ? Item : nullptr);
}

void PEditorHUD::OnActorDestroyed(PActor* Actor)
{
	if (Actor && ActorParamView != nullptr)
	{
		SelectPanel->RemoveAllChildren();
		GWorld->DestroyWidget(ActorParamView);
	}
}
