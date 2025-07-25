#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "Engine/Actors/Portal.h"
#include "Engine/Input.h"
#include "Engine/Serialization.h"
#include "Interface/HUD.h"
#include "Interface/Dropdown.h"
#include "Interface/GridView.h"
#include "Interface/Group.h"
#include "Interface/Panel.h"
#include "Interface/ScrollArea.h"
#include "Interface/Spinner.h"
#include "EditorView.h"
#include "../Runtime/Engine/Actors/Game/SignPost.h"

static PPanel* MainPanel;

static std::vector<std::string> EditModeStrings = {
	"Select",
	"Tiles",
	"Actors",
};
static PGroup* TileGroup;
static PGroup* ActorGroup;

static std::vector<SActorItem> PlaceableActors = {
	{"Portal"},
	{"SignPost"},
	{"Hill (South)"},
	{"Hill (West)"},
	{"Hill (East)"},
	{"Water"},
};

std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{"JSON", "JSON"},
};

PEditorGame* GetEditorGame()
{
	return dynamic_cast<PEditorGame*>(GetGame());
}

bool PEditorGame::PreStart()
{
	GetSettings()->DebugDraw = true;

	const auto EditorView = mWorld->ConstructActor<PEditorView>();
	if (!EditorView)
	{
		LogError("Failed to create Editor View");
	}

	if (!LoadAllTilesets())
	{
		return false;
	}
	SetupInterface();

	return true;
}

void PEditorGame::Start()
{
	PGame::Start();
	mWorld->ActorClicked.AddRaw(this, &PEditorGame::OnActorClicked);
}

void PEditorGame::OnDropdownClicked(SDropdownItemData* Data) const
{
	LogDebug("Item clicked: {}", Data->Index);
}

void PEditorGame::SetupInterface()
{
	MainPanel = mWorld->ConstructWidget<PPanel>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(340);

	// Files

	const auto FileGroup = mWorld->ConstructWidget<PGroup>("File");
	FileGroup->SetResizeModeH(RM_Fit);
	FileGroup->SetLayoutMode(LM_Vertical);

	const auto NewButton    = mWorld->ConstructWidget<PButton>("New", this, &PEditorGame::OnNewButtonClicked);
	const auto CreateButton = mWorld->ConstructWidget<PButton>("Create", this, &PEditorGame::OnCreateButtonClicked);
	const auto SizeXSpinner = mWorld->ConstructWidget<PSpinner>(mNewMapSizeX);
	SizeXSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeXChanged);
	const auto SizeYSpinner = mWorld->ConstructWidget<PSpinner>(mNewMapSizeY);
	SizeYSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeYChanged);
	const auto SaveButton = mWorld->ConstructWidget<PButton>("Save", this, &PEditorGame::OnSaveButtonClicked);
	const auto LoadButton = mWorld->ConstructWidget<PButton>("Load", this, &PEditorGame::OnLoadButtonClicked);

	FileGroup->AddChild(NewButton);
	FileGroup->AddChild(CreateButton);
	FileGroup->AddChild(SizeXSpinner);
	FileGroup->AddChild(SizeYSpinner);
	FileGroup->AddChild(SaveButton);
	FileGroup->AddChild(LoadButton);

	// Edit

	const auto EditGroup = mWorld->ConstructWidget<PGroup>("Edit");
	EditGroup->SetResizeModeH(RM_Fit);
	EditGroup->SetLayoutMode(LM_Vertical);

	const auto EditMode = mWorld->ConstructWidget<PDropdown>(EditModeStrings);
	EditGroup->AddChild(EditMode);
	EditMode->SetResizeModeH(RM_Fixed);
	EditMode->SetFixedHeight(20);
	EditMode->ItemClicked.AddRaw(this, &PEditorGame::OnEditModeClicked);

	// Tiles

	TileGroup = mWorld->ConstructWidget<PGroup>("Tiles");
	TileGroup->SetLayoutMode(LM_Vertical);

	auto ScrollArea = mWorld->ConstructWidget<PScrollArea>();
	TileGroup->AddChild(ScrollArea);

	// Create a button group for all tiles across all tilesets
	mTilesetViewButtonGroup = mWorld->ConstructWidget<PButtonGroup>();

	// Construct each widget for each tile in each tileset
	for (const auto Tileset : GetTilesets())
	{
		auto TilesetView = ConstructTilesetView(Tileset);
		ScrollArea->AddChild(TilesetView);
		mTilesetViews[Tileset->Name] = TilesetView;
	}

	// Actors

	mActorViewButtonGroup = ConstructWidget<PButtonGroup>();
	ActorGroup            = mWorld->ConstructWidget<PGroup>("Actors");
	ActorGroup->SetLayoutMode(LM_Vertical);

	auto ActorView = ConstructActorView();
	ActorGroup->AddChild(ActorView);

	// Main panel

	MainPanel->AddChild(FileGroup);
	MainPanel->AddChild(EditGroup);

	// Main canvas

	const auto HUD = mWorld->GetHUD();
	HUD->AddChild(MainPanel);
}

PGridView* PEditorGame::ConstructTilesetView(STileset* Tileset)
{
	const int ItemSize   = 20;
	const int ViewWidth  = Tileset->Width * ItemSize;
	const int ViewHeight = Tileset->Height * ItemSize;

	PGridView* GridView = mWorld->ConstructWidget<PGridView>();
	GridView->SetGridCount(Tileset->Width);
	GridView->SetFixedWidth(ViewWidth);
	GridView->SetResizeMode(RM_Fixed, RM_Fixed);
	GridView->SetFixedHeight(ViewHeight);

	auto TilesetTexture = Tileset->Texture;

	for (auto& Item : Tileset->Tiles)
	{
		// Create the button item
		auto GridItem   = GridView->AddItem<PButton>(TilesetTexture);
		auto Button     = GridItem->GetWidget<PButton>();
		Button->Padding = {0};
		Button->SetResizeMode(RM_Fixed, RM_Fixed);
		Button->SetFixedSize(ItemSize);
		Button->SetCheckable(true);
		Button->SetCustomData(&Item);
		Button->SetUseSourceRect(true);
		Button->SetSourceRect(Item.GetSourceRect());
		Button->Checked.AddRaw(this, &PEditorGame::OnTilesetButtonChecked);

		mTilesetViewButtonGroup->AddButton(Button);
	}

	return GridView;
}

PGridView* PEditorGame::ConstructActorView()
{
	PGridView* GridView = mWorld->ConstructWidget<PGridView>();
	GridView->Padding   = {5};
	GridView->SetGridCount(2);

	for (auto& Item : PlaceableActors)
	{
		const int ItemSize = 40;
		// Create the button item
		auto GridItem = GridView->AddItem<PButton>(Item.Name);
		auto Button   = GridItem->GetWidget<PButton>();
		Button->SetResizeMode(RM_Grow, RM_Fixed);
		Button->SetFixedHeight(ItemSize);
		Button->SetCheckable(true);
		Button->SetCustomData(&Item);
		Button->Checked.AddRaw(this, &PEditorGame::OnActorButtonChecked);

		mActorViewButtonGroup->AddButton(Button);
	}

	return GridView;
}

void PEditorGame::AddInputContext(uint8_t InputContext)
{
	mInputContext |= InputContext;
}

void PEditorGame::RemoveInputContext(uint8_t InputContext)
{
	mInputContext &= ~InputContext;
}

bool PEditorGame::HasInputContext(uint8_t InputContext)
{
	return (mInputContext & InputContext) == InputContext;
}

void PEditorGame::OnKeyDown(SInputEvent* Event)
{
	PGame::OnKeyDown(Event);
	if (Event->Consumed)
	{
		return;
	}

	switch (Event->KeyDown)
	{
	case SDLK_LSHIFT: if (HasInputContext(IC_Tile))
		{
			mBrushMode = BM_Copy;
		}
		break;
	case SDLK_LCTRL: if (HasInputContext(IC_Tile))
		{
			mBrushMode = BM_Fill;
		}
		break;
	default: break;
	}
}

void PEditorGame::OnKeyUp(SInputEvent* Event)
{
	PGame::OnKeyUp(Event);
	if (Event->Consumed)
	{
		return;
	}

	switch (Event->KeyUp)
	{
	case SDLK_DELETE:
		// ReSharper disable once CppDFAConstantConditions
		if (HasInputContext(IC_Select) && mCurrentMap)
		{
			// Remove the map from the list of maps
			mMaps.erase(std::ranges::remove(mMaps, mCurrentMap).begin());
			// Destroy the map actor
			GetWorld()->DestroyActor(mCurrentMap);
			// Set the current map to null
			mCurrentMap = nullptr;
			Event->Consume();
		}
		break;
	case SDLK_UP:
	case SDLK_DOWN: if (HasInputContext(IC_Tile))
		{
			mBrushSize = Event->KeyUp == SDLK_UP ? BS_Large : BS_Small;
		}
		break;
	case SDLK_LSHIFT:
	case SDLK_LCTRL: if (HasInputContext(IC_Tile))
		{
			mBrushMode = BM_Default;
		}
		break;
	default: break;
	}
}

void PEditorGame::OnCreateButtonClicked()
{
	int TileCountX = mNewMapSizeX * 2;
	int TileCountY = mNewMapSizeY * 2;
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
			auto Tileset = mCurrentTileset ? mCurrentTileset->Name : TILESET_1;
			JsonData["Tiles"].push_back({
				{"Index", 0},
				{"Tileset", Tileset}
			});
		}
	}
	ConstructMap(JsonData);
}

void PEditorGame::OnSaveButtonClicked()
{
	JSON Json;

	for (const auto Actor : mWorld->GetActors())
	{
		if (auto Map = dynamic_cast<PMap*>(Actor))
		{
			Json = Map->Serialize();
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

void PEditorGame::OnLoadButtonClicked()
{
	std::string FileName;

	if (!Files::GetOpenFileName(&FileName, gDefaultFilters))
	{
		return;
	}

	std::string Data;
	if (!Files::ReadFile(FileName, Data))
	{
		return;
	}

	const JSON JsonData = JSON::parse(Data.data());
	Serialization::Deserialize(JsonData);
}

void PEditorGame::OnEditModeClicked(SDropdownItemData* DropdownItemData)
{
	mInputContext = 0;
	switch (DropdownItemData->Index)
	{
	case 0: // IC_Select
		AddInputContext(IC_Select);
		MainPanel->RemoveChild(TileGroup);
		MainPanel->RemoveChild(ActorGroup);
		break;
	case 1: // IC_Tile
		AddInputContext(IC_Tile);
		MainPanel->AddChild(TileGroup);
		MainPanel->RemoveChild(ActorGroup);
		break;
	case 2: // IC_Actor
		AddInputContext(IC_Actor);
		MainPanel->RemoveChild(TileGroup);
		MainPanel->AddChild(ActorGroup);
		break;
	default:
		LogError("Invalid dropdown item: {}", DropdownItemData->Index);
		break;
	}
}

void PEditorGame::OnTilesetButtonChecked(bool State)
{
	auto Sender = PWidget::GetSender<PButton>();
	auto Item   = Sender->GetCustomData<STileItem>();

	mCurrentTileset = Item->Tileset;
	LogDebug("Set new tileset: {}", mCurrentTileset->Name);
	if (State)
	{
		mCurrentTilesetItem = Item;
	}
	else
	{
		mCurrentTilesetItem = nullptr;
	}
	LogDebug("Set new tileset item: {}", mCurrentTilesetItem ? mCurrentTilesetItem->Index : -1);
}

void PEditorGame::OnActorButtonChecked(bool State)
{
	auto Sender = PWidget::GetSender<PButton>();
	auto Item   = Sender->GetCustomData<SActorItem>();
	if (State)
	{
		mCurrentActorItem = Item;
	}
	else
	{
		mCurrentActorItem = nullptr;
	}
	LogDebug("Set new actor item: {}", mCurrentActorItem ? mCurrentActorItem->Name.c_str() : "None");
}

void PEditorGame::UpdateSelection(PActor* ClickedActor)
{
	if (auto Map = dynamic_cast<PMap*>(ClickedActor))
	{
		Map->ToggleSelected();
		for (auto Actor : GetWorld()->GetActors())
		{
			if (Map->GetInternalName() == Actor->GetInternalName())
			{
				continue;
			}
			Actor->SetSelected(false);
		}
		mCurrentMap = Map;
	}
}

void PEditorGame::OnActorClicked(PActor* ClickedActor)
{
	auto Map = GetCurrentMap();
	if (HasInputContext(IC_Select))
	{
		UpdateSelection(ClickedActor);
	}
	else if (HasInputContext(IC_Tile))
	{
		if (!mCurrentTilesetItem)
		{
			LogWarning("No tileset item selected.");
			return;
		}
		if (Map)
		{
			switch (mBrushMode)
			{
			case BM_Default:
			case BM_Copy: PaintTile(Map->GetTileUnderMouse());
				break;
			case BM_Fill: for (auto Tile : Map->GetTiles())
				{
					Tile->Index   = mCurrentTilesetItem->Index;
					Tile->Tileset = mCurrentTileset;
				}
				break;
			}
		}
	}
	else if (HasInputContext(IC_Actor))
	{
		if (!mCurrentActorItem)
		{
			LogWarning("No actor item selected.");
			return;
		}
		auto Position = Map->GetTileUnderMouse()->GetPosition();
		auto Actors   = mWorld->GetActorsAtPosition(Position);
		if (Actors.size() == 0)
		{
			PActor* NewActor = nullptr;
			if (mCurrentActorItem->Name == "Portal")
			{
				NewActor = SpawnActor<PPortal>();
			}
			else if (mCurrentActorItem->Name == "SignPost")
			{
				NewActor = SpawnActor<PSignPost>();
			}
			else
			{
				LogWarning("Actor placement not implemented for {}", mCurrentActorItem->Name);
				return;
			}

			if (NewActor)
			{
				LogDebug("Placing {}", mCurrentActorItem->Name.c_str());
				NewActor->SetPosition(Position);
				Map->AddChild(NewActor);
			}
		}
	}
}

void PEditorGame::OnNewButtonClicked()
{
	for (auto Actor : mWorld->GetActors())
	{
		mWorld->DestroyActor(Actor);
	}
	mMaps.clear();
	mCurrentMap = nullptr;
}

void PEditorGame::AddMap(PMap* Map)
{
	mMaps.emplace_back(Map);
	SetCurrentMap(Map);
}

void PEditorGame::SetCurrentMap(PMap* Map)
{
	mCurrentMap = Map;
}

void PEditorGame::ConstructMap(const JSON& JsonData)
{
	// Create the map
	const auto Map = SpawnActor<PMap>(JsonData);
	if (!Map)
	{
		LogError("Failed to create map");
		return;
	}
	AddMap(Map);
}

void PEditorGame::ActorSelected(PActor* Actor)
{
	if (const auto Map = dynamic_cast<PMap*>(Actor))
	{
		// Deselect all other maps
		for (const auto C : mWorld->GetActorsOfType<PMap>())
		{
			if (C == Map)
			{
				continue; // Skip the currently selected map
			}
			C->SetSelected(false);
		}

		Map->GetSelected() ? SetCurrentMap(Map) : SetCurrentMap(nullptr);
	}
}

void PEditorGame::PaintTile(STile* Tile)
{
	if (!Tile)
	{
		return;
	}

	// Fill all tiles
	PMap* Map = Tile->Map;

	// Set hit tile
	Tile->Tileset = mCurrentTileset;
	Tile->Index   = mCurrentTilesetItem->Index;

	// Set adjacent tiles
	if (mBrushSize == BS_Large)
	{
		auto MousePosition = GetRenderer()->GetMouseWorldPosition();

		// TODO: Clean this up
		if (auto Tile2 = Map->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, 0)))
		{
			Tile2->Tileset = mCurrentTileset;
			Tile2->Index   = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + 1 : mCurrentTilesetItem->Index;
		}
		if (auto Tile3 = Map->GetTileAtPosition(MousePosition + FVector2(0, TILE_SIZE)))
		{
			Tile3->Tileset = mCurrentTileset;
			Tile3->Index   = mBrushMode == BM_Copy
				               ? mCurrentTilesetItem->Index + Tile->Tileset->Width
				               : mCurrentTilesetItem->Index;
		}
		if (auto Tile4 = Map->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, TILE_SIZE)))
		{
			Tile4->Tileset = mCurrentTileset;
			Tile4->Index   = mBrushMode == BM_Copy
				               ? mCurrentTilesetItem->Index + Tile->Tileset->Width + 1
				               : mCurrentTilesetItem->Index;
		}
	}
}
