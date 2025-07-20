#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"
#include "Interface/Canvas.h"
#include "Interface/Dropdown.h"
#include "Interface/GridView.h"
#include "Interface/Group.h"
#include "Interface/Panel.h"
#include "Interface/ScrollArea.h"
#include "Interface/Spinner.h"

static PGroup* TileGroup = nullptr;

std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{ "json", "JSON" },
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
	const auto MainPanel = mWorld->ConstructWidget<PPanel>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(340);

	// Files

	const auto FileGroup = mWorld->ConstructWidget<PGroup>("File");
	FileGroup->SetResizeModeH(RM_Fit);
	FileGroup->SetLayoutMode(LM_Vertical);

	const auto NewButton = mWorld->ConstructWidget<PButton>("New", this, &PEditorGame::OnNewButtonClicked);
	const auto CreateButton = mWorld->ConstructWidget<PButton>("Create", this, &PEditorGame::OnCreateButtonClicked);
	const auto SizeXSpinner = mWorld->ConstructWidget<PSpinner>(mNewChunkSizeX);
	SizeXSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeXChanged);
	const auto SizeYSpinner = mWorld->ConstructWidget<PSpinner>(mNewChunkSizeY);
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
	const auto EditModeSelect = mWorld->ConstructWidget<PButton>("Select", this, &PEditorGame::OnSelectButtonChecked);
	EditModeSelect->SetCheckable(true);
	const auto EditModeTile = mWorld->ConstructWidget<PButton>("Tile", this, &PEditorGame::OnTileButtonChecked);
	EditModeTile->SetCheckable(true);
	const auto EditModeButtonGroup = mWorld->ConstructWidget<PButtonGroup>();
	EditModeButtonGroup->AddButton(EditModeSelect);
	EditModeButtonGroup->AddButton(EditModeTile);
	EditGroup->AddChild(EditModeSelect);
	EditGroup->AddChild(EditModeTile);

	// Tiles

	TileGroup = mWorld->ConstructWidget<PGroup>("Tiles");
	TileGroup->SetLayoutMode(LM_Vertical);
	TileGroup->SetVisible(false);

	auto ScrollArea = mWorld->ConstructWidget<PScrollArea>();
	TileGroup->AddChild(ScrollArea);

	mTilesetViewButtonGroup = mWorld->ConstructWidget<PButtonGroup>();

	for (const auto Tileset : GetTilesets())
	{
		auto View = ConstructTilesetView(Tileset);
		ScrollArea->AddChild(View);
		mTilesetViews[Tileset->Name] = View;
	}

	// Main panel

	MainPanel->AddChild(FileGroup);
	MainPanel->AddChild(EditGroup);
	MainPanel->AddChild(TileGroup);

	// Main canvas

	const auto MainCanvas = mWorld->ConstructWidget<PCanvas>();
	MainCanvas->AddChild(MainPanel);

	mWorld->SetRootWidget(MainCanvas);
}

PGridView* PEditorGame::ConstructTilesetView(STileset* Tileset)
{
	const int ItemSize = 20;
	const int ViewWidth = Tileset->Width * ItemSize;
	const int ViewHeight = Tileset->Height * ItemSize;

	PGridView* GridView = mWorld->ConstructWidget<PGridView>();
	GridView->SetGridWidth(Tileset->Width);
	GridView->SetVisible(true);
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
		Button->Checked.AddRaw(this, &PEditorGame::OnTilesetButtonChecked);

		mTilesetViewButtonGroup->AddButton(Button);
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
		case SDLK_LSHIFT:
			if (HasInputContext(IC_Tile))
			{
				mBrushMode = BM_Copy;
			}
			break;
		case SDLK_LCTRL:
			if (HasInputContext(IC_Tile))
			{
				mBrushMode = BM_Fill;
			}
			break;
		default:
			break;
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
			if (HasInputContext(IC_Select) && mCurrentChunk)
			{
				// Remove the chunk from the list of chunks
				mChunks.erase(std::ranges::remove(mChunks, mCurrentChunk).begin());
				// Destroy the chunk actor
				GetWorld()->DestroyActor(mCurrentChunk);
				// Set the current chunk to null
				mCurrentChunk = nullptr;
				Event->Consume();
			}
			break;
		case SDLK_UP:
		case SDLK_DOWN:
			if (HasInputContext(IC_Tile))
			{
				mBrushSize = Event->KeyUp == SDLK_UP ? BS_Large : BS_Small;
			}
			break;
		case SDLK_LSHIFT:
		case SDLK_LCTRL:
			if (HasInputContext(IC_Tile))
			{
				mBrushMode = BM_Default;
			}
			break;
		default:
			break;
	}
}

void PEditorGame::OnCreateButtonClicked()
{
	int TileCountX = mNewChunkSizeX * 2;
	int TileCountY = mNewChunkSizeY * 2;
	LogDebug("Creating new chunk: [{}, {}]", TileCountX, TileCountY);

	json JsonData = {
		{ "Position", { 0, 0 }   },
		{ "SizeX",	   TileCountX },
		{ "SizeY",	   TileCountY },
	};
	for (int X = 0; X < TileCountX; ++X)
	{
		for (int Y = 0; Y < TileCountY; ++Y)
		{
			auto Tileset = mCurrentTileset ? mCurrentTileset->Name : "Tileset1";
			JsonData["Tiles"].push_back({
				{ "Index",   0		 },
				{ "Tileset", Tileset }
			   });
		}
	}
	ConstructChunk(JsonData);
}

void PEditorGame::OnSaveButtonClicked()
{
	PSerializer Serializer;

	for (const auto Actor : mWorld->GetActors())
	{
		Serializer.Serialize(Actor);
	}

	std::string FileName;
	if (Files::GetSaveFileName(&FileName, gDefaultFilters))
	{
		LogDebug("Saving to file: {}", FileName);
		Files::WriteFile(FileName, Serializer.GetSerializedData().dump(4));
	}
}
void PEditorGame::OnLoadButtonClicked()
{
	PSerializer Serializer;
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

	const json JsonData = json::parse(Data.data());
	Serializer.Deserialize(JsonData);
}

void PEditorGame::OnSelectButtonChecked(bool State)
{
	RemoveInputContext(IC_Tile);
	State ? AddInputContext(IC_Select) : RemoveInputContext(IC_Select);

	TileGroup->SetVisible(false);
}

void PEditorGame::OnTileButtonChecked(bool State)
{
	RemoveInputContext(IC_Select);
	State ? AddInputContext(IC_Tile) : RemoveInputContext(IC_Tile);

	TileGroup->SetVisible(State);
}

void PEditorGame::OnTilesetButtonChecked(bool State)
{
	auto Sender = PWidget::GetSender<PButton>();
	auto Item = Sender->GetCustomData<STileItem>();

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

void PEditorGame::UpdateSelection(PActor* ClickedActor)
{
	if (auto Chunk = dynamic_cast<PChunk*>(ClickedActor))
	{
		Chunk->ToggleSelected();
		for (auto Actor : GetWorld()->GetActors())
		{
			if (Chunk->GetInternalName() == Actor->GetInternalName())
			{
				continue;
			}
			Actor->SetSelected(false);
		}
		mCurrentChunk = Chunk;
	}
}

void PEditorGame::OnActorClicked(PActor* ClickedActor)
{
	if (HasInputContext(IC_Select))
	{
		UpdateSelection(ClickedActor);
	}
	if (HasInputContext(IC_Tile))
	{
		if (!mCurrentTilesetItem)
		{
			LogWarning("No tileset item selected.");
			return;
		}
		if (auto Chunk = dynamic_cast<PChunk*>(ClickedActor))
		{
			switch (mBrushMode)
			{
				case BM_Default:
				case BM_Copy:
					PaintTile(Chunk->GetTileUnderMouse());
					break;
				case BM_Fill:
					for (auto Tile : Chunk->GetTiles())
					{
						Tile->Index = mCurrentTilesetItem->Index;
						Tile->Tileset = mCurrentTileset;
					}
					break;
			}
		}
	}
}

void PEditorGame::OnNewButtonClicked()
{
	for (auto Chunk : mChunks)
	{
		mWorld->DestroyActor(Chunk);
	}
	mChunks.clear();
	mCurrentChunk = nullptr;
}

void PEditorGame::AddChunk(PChunk* Chunk)
{
	mChunks.emplace_back(Chunk);
	SetCurrentChunk(Chunk);
}

void PEditorGame::SetCurrentChunk(PChunk* Chunk)
{
	mCurrentChunk = Chunk;
}

void PEditorGame::ConstructChunk(const json& JsonData)
{
	// Create the chunk
	const auto Chunk = mWorld->SpawnActor<PChunk>(JsonData);
	if (!Chunk)
	{
		LogError("Failed to create chunk");
		return;
	}
	AddChunk(Chunk);
}

void PEditorGame::ActorSelected(PActor* Actor)
{
	if (const auto Chunk = dynamic_cast<PChunk*>(Actor))
	{
		// Deselect all other chunks
		for (const auto C : mWorld->GetActorsOfType<PChunk>())
		{
			if (C == Chunk)
			{
				continue; // Skip the currently selected chunk
			}
			C->SetSelected(false);
		}

		Chunk->GetSelected() ? SetCurrentChunk(Chunk) : SetCurrentChunk(nullptr);
	}
}

void PEditorGame::PaintTile(STile* Tile)
{
	if (!Tile)
	{
		return;
	}

	// Fill all tiles
	PChunk* Chunk = Tile->Chunk;

	// Set hit tile
	Tile->Tileset = mCurrentTileset;
	Tile->Index = mCurrentTilesetItem->Index;

	// Set adjacent tiles
	if (mBrushSize == BS_Large)
	{
		auto MousePosition = GetRenderer()->GetMouseWorldPosition();

		// TODO: Clean this up
		if (auto Tile2 = Chunk->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, 0)))
		{
			Tile2->Tileset = mCurrentTileset;
			Tile2->Index = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + 1 : mCurrentTilesetItem->Index;
		}
		if (auto Tile3 = Chunk->GetTileAtPosition(MousePosition + FVector2(0, TILE_SIZE)))
		{
			Tile3->Tileset = mCurrentTileset;
			Tile3->Index = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + Tile->Tileset->Width : mCurrentTilesetItem->Index;
		}
		if (auto Tile4 = Chunk->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, TILE_SIZE)))
		{
			Tile4->Tileset = mCurrentTileset;
			Tile4->Index = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + Tile->Tileset->Width + 1 : mCurrentTilesetItem->Index;
		}
	}
}