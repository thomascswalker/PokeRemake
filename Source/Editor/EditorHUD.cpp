// ReSharper disable CppDFANullDereference
#include "EditorHUD.h"

#include "ActorManager.h"
#include "EditorGame.h"

#include "Application/Application.h"

#include "Engine/Game.h"
#include "Engine/MapManager.h"
#include "Engine/World.h"
#include "Interface/ButtonGroup.h"
#include "Interface/Group.h"
#include "Interface/Menu.h"
#include "Interface/Panel.h"
#include "Interface/ScrollArea.h"
#include "Interface/Spinner.h"

static PWorld* World           = nullptr;
static PEditorGame* EditorGame = nullptr;

static PPanel* MainPanel  = nullptr;
static PGroup* EditGroup  = nullptr;
static PGroup* TileGroup  = nullptr;
static PGroup* ActorGroup = nullptr;
static std::map<std::string, PGridView*> TilesetViews;
static PButtonGroup* TilesetViewButtonGroup = nullptr;
static PButtonGroup* ActorViewButtonGroup   = nullptr;

std::vector<SActorItem> gPlaceableActors{};
std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{"JSON", "JSON"},
};
static std::vector<std::string> gEditModeStrings = {
	"Select",
	"Tiles",
	"Actors",
};

bool PEditorHUD::PreStart()
{
	World = GetWorld();
	assert(World != nullptr);
	EditorGame = dynamic_cast<PEditorGame*>(GetGame());
	assert(EditorGame != nullptr);

	ActorManager::LoadActorDefs();
	for (auto& [K, V] : ActorManager::GetActorDefs().items())
	{
		gPlaceableActors.push_back(SActorItem(K, V));
	}

	if (!LoadAllTilesets())
	{
		return false;
	}
	SetupInterface();

	return true;
}

void PEditorHUD::SetupInterface()
{
	// Vertical layout to force the file menu to the top
	mLayoutMode = LM_Vertical;

	// File Menu
	auto MenuBar = ConstructWidget<PMenuBar>();
	MenuBar->AddMenu("File", {"New", "Load", "Save", "Exit"});
	MenuBar->AddMenu("Edit", {"Select", "Tile", "Actor"});

	AddChild(MenuBar);

	// Main panel
	MainPanel = World->ConstructWidget<PPanel>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(340);
	// Files
	const auto FileGroup = World->ConstructWidget<PGroup>("File");
	FileGroup->SetResizeModeH(RM_Fit);
	FileGroup->SetLayoutMode(LM_Vertical);
	const auto NewButton    = World->ConstructWidget<PButton>("New", this, &PEditorHUD::OnNewButtonClicked);
	const auto CreateButton = World->ConstructWidget<PButton>("Create", this, &PEditorHUD::OnCreateButtonClicked);
	const auto SizeXSpinner = World->ConstructWidget<PSpinner>(mNewMapSizeX);
	SizeXSpinner->ValueChanged.AddRaw(this, &PEditorHUD::OnSizeXChanged);
	const auto SizeYSpinner = World->ConstructWidget<PSpinner>(mNewMapSizeY);
	SizeYSpinner->ValueChanged.AddRaw(this, &PEditorHUD::OnSizeYChanged);
	const auto SaveButton = World->ConstructWidget<PButton>("Save", this, &PEditorHUD::OnSaveButtonClicked);
	const auto LoadButton = World->ConstructWidget<PButton>("Load", this, &PEditorHUD::OnLoadButtonClicked);
	FileGroup->AddChild(NewButton);
	FileGroup->AddChild(CreateButton);
	FileGroup->AddChild(SizeXSpinner);
	FileGroup->AddChild(SizeYSpinner);
	FileGroup->AddChild(SaveButton);
	FileGroup->AddChild(LoadButton);
	// Edit
	EditGroup = World->ConstructWidget<PGroup>("Edit");
	EditGroup->SetResizeModeH(RM_Fit);
	EditGroup->SetLayoutMode(LM_Vertical);
	const auto EditMode = World->ConstructWidget<PDropdown>(gEditModeStrings);
	EditGroup->AddChild(EditMode);
	EditMode->SetResizeModeH(RM_Fixed);
	EditMode->SetFixedHeight(20);
	EditMode->ItemClicked.AddRaw(this, &PEditorHUD::OnEditModeClicked);
	// Tiles
	TileGroup = World->ConstructWidget<PGroup>("Tiles");
	TileGroup->SetLayoutMode(LM_Vertical);
	auto ScrollArea = World->ConstructWidget<PScrollArea>();
	TileGroup->AddChild(ScrollArea);
	// Create a button group for all tiles across all tilesets
	TilesetViewButtonGroup = World->ConstructWidget<PButtonGroup>();
	// Construct each widget for each tile in each tileset
	for (const auto Tileset : GetTilesets())
	{
		auto TilesetView = ConstructTilesetView(Tileset);
		ScrollArea->AddChild(TilesetView);
		TilesetViews[Tileset->Name] = TilesetView;
	}
	// Actors
	ActorViewButtonGroup = ConstructWidget<PButtonGroup>();
	ActorGroup           = World->ConstructWidget<PGroup>("Actors");
	ActorGroup->SetLayoutMode(LM_Vertical);
	auto ActorView = ConstructActorView();
	ActorGroup->AddChild(ActorView);
	// Main panel

	MainPanel->AddChild(FileGroup);
	MainPanel->AddChild(EditGroup);

	AddChild(MainPanel);
}

PGridView* PEditorHUD::ConstructTilesetView(STileset* Tileset)
{
	if (!World || !EditorGame)
	{
		return nullptr;
	}
	const int ItemSize   = 20;
	const int ViewWidth  = Tileset->Width * ItemSize;
	const int ViewHeight = Tileset->Height * ItemSize;

	PGridView* GridView = World->ConstructWidget<PGridView>();
	GridView->SetGridCount(Tileset->Width);
	GridView->SetFixedWidth(ViewWidth);
	GridView->SetResizeMode(RM_Fixed, RM_Fixed);
	GridView->SetFixedHeight(ViewHeight);

	auto TilesetTexture = Tileset->Texture;

	for (auto& Item : Tileset->Tiles)
	{
		// Create the button item
		auto GridItem    = GridView->AddItem<PButton>(TilesetTexture);
		auto Button      = GridItem->GetWidget<PButton>();
		Button->mPadding = {0};
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
	if (!World || !EditorGame)
	{
		return nullptr;
	}
	PGridView* GridView = World->ConstructWidget<PGridView>();
	GridView->mPadding  = {5};
	GridView->SetGridCount(1);

	for (auto& ActorItem : gPlaceableActors)
	{
		const int ItemSize = 40;
		// Create the button item
		auto GridItem = GridView->AddItem<PButton>(ActorItem.Name);
		auto Button   = GridItem->GetWidget<PButton>();
		Button->SetResizeMode(RM_Grow, RM_Fixed);
		Button->SetFixedHeight(ItemSize);
		Button->SetCheckable(true);
		Button->SetCustomData(&ActorItem);
		Button->Checked.AddRaw(this, &PEditorHUD::OnActorButtonChecked);

		ActorViewButtonGroup->AddButton(Button);
	}

	return GridView;
}

void PEditorHUD::OnSizeXChanged(float Value)
{
	mNewMapSizeX = Value;
}

void PEditorHUD::OnSizeYChanged(float Value)
{
	mNewMapSizeY = Value;
}

void PEditorHUD::OnEditModeClicked(SDropdownItemData* DropdownItemData)
{
	switch (DropdownItemData->Index)
	{
	case 0: // IC_Select
		SetInputContext(IC_Select);
		MainPanel->RemoveChild(TileGroup);
		MainPanel->RemoveChild(ActorGroup);
		break;
	case 1: // IC_Tile
		SetInputContext(IC_Tile);
		MainPanel->AddChild(TileGroup);
		MainPanel->RemoveChild(ActorGroup);
		break;
	case 2: // IC_Actor
		SetInputContext(IC_Actor);
		MainPanel->RemoveChild(TileGroup);
		MainPanel->AddChild(ActorGroup);
		break;
	default:
		LogError("Invalid dropdown item: {}", DropdownItemData->Index);
		break;
	}
}

void PEditorHUD::OnNewButtonClicked()
{
	World->DestroyAllActors();
	PMapManager::ClearMaps();
}

void PEditorHUD::OnCreateButtonClicked()
{
	auto CurrentTileset = EditorGame->GetCurrentTileset();
	int TileCountX      = mNewMapSizeX * 2;
	int TileCountY      = mNewMapSizeY * 2;
	LogDebug("Creating new map: [{}, {}]", TileCountX, TileCountY);

	JSON JsonData = {
		{"MapName", "NewMap"},
		{"Position", {0, 0}},
		{"SizeX", TileCountX},
		{"SizeY", TileCountY},
	};
	for (int X = 0; X < TileCountX; ++X)
	{
		for (int Y = 0; Y < TileCountY; ++Y)
		{
			auto Tileset = CurrentTileset ? CurrentTileset->Name : TILESET_1;
			JsonData["Tiles"].push_back({
				{"Index", 0},
				{"Tileset", Tileset}
			});
		}
	}

	PMapManager::ConstructMap(JsonData);
}

void PEditorHUD::OnSaveButtonClicked()
{
	JSON Json;

	for (const auto Actor : World->GetActors())
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

	PMapManager::LoadMap(FileName, true);
}

void PEditorHUD::OnTilesetButtonChecked(bool State)
{
	auto Sender = GetSender<PButton>();
	auto Item   = Sender->GetCustomData<STileItem>();

	EditorGame->SetCurrentTileset(Item->Tileset);
	EditorGame->SetCurrentTilesetItem(State ? Item : nullptr);
}

void PEditorHUD::OnActorButtonChecked(bool State)
{
	auto Sender = GetSender<PButton>();
	auto Item   = Sender->GetCustomData<SActorItem>();
	EditorGame->SetCurrentActorItem(State ? Item : nullptr);
}
