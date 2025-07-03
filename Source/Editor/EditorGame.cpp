#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"
#include "Interface/Box.h"
#include "Interface/ClipArea.h"
#include "Interface/Group.h"
#include "Interface/Spinner.h"

#define NEW_GRID_SIZE 5

std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{ "json", "JSON" },
};

PEditorGame* GetEditorGame()
{
	return dynamic_cast<PEditorGame*>(GetGame());
}

void PEditorGame::PreStart()
{
	GetSettings()->bDebugDraw = true;

	const auto EV = mWorld->ConstructActor<PEditorView>();
	if (!EV)
	{
		LogError("Failed to create Editor View");
	}

	ConstructInterface();
}

void PEditorGame::Start()
{
	mWorld->Start();
	FindActiveCamera();
}

void PEditorGame::ConstructInterface()
{
	const auto MainPanel = mWorld->ConstructWidget<PBox>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(150);

	// Files
	const auto FileGroup = mWorld->ConstructWidget<PGroup>("File");
	FileGroup->SetResizeModeH(RM_Fit);
	FileGroup->SetLayoutMode(LM_Vertical);

	const auto CreateButton = mWorld->ConstructWidget<PButton>("Create", this, &PEditorGame::OnCreateButtonClicked);
	const auto SizeXSpinner = mWorld->ConstructWidget<PSpinner>(5);
	SizeXSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeXChanged);
	const auto SizeYSpinner = mWorld->ConstructWidget<PSpinner>(5);
	SizeYSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeYChanged);
	const auto SaveButton = mWorld->ConstructWidget<PButton>("Save", this, &PEditorGame::OnSaveButtonClicked);
	SaveButton->SetFontSize(WIDGET_FONT_SIZE);
	const auto LoadButton = mWorld->ConstructWidget<PButton>("Load", this, &PEditorGame::OnLoadButtonClicked);
	LoadButton->SetFontSize(WIDGET_FONT_SIZE);
	FileGroup->AddChild(CreateButton);
	FileGroup->AddChild(SizeXSpinner);
	FileGroup->AddChild(SizeYSpinner);
	FileGroup->AddChild(SaveButton);
	FileGroup->AddChild(LoadButton);

	// Edit
	const auto EditGroup = mWorld->ConstructWidget<PGroup>("Edit");
	EditGroup->SetResizeModeH(RM_Fit);
	EditGroup->SetLayoutMode(LM_Vertical);
	const auto EditModeChunk = mWorld->ConstructWidget<PButton>("Chunk", this, &PEditorGame::OnSelectButtonChecked);
	EditModeChunk->SetCheckable(true);
	const auto EditModeTileType = mWorld->ConstructWidget<PButton>("Tile Type", this, &PEditorGame::OnTileButtonChecked);
	EditModeTileType->SetCheckable(true);
	const auto EditModeTileSprite = mWorld->ConstructWidget<PButton>("Tile Sprite", this, &PEditorGame::OnTileButtonChecked);
	EditModeTileSprite->SetCheckable(true);
	const auto EditModeButtonGroup = mWorld->ConstructWidget<PButtonGroup>();
	EditModeButtonGroup->AddButton(EditModeChunk);
	EditModeButtonGroup->AddButton(EditModeTileType);
	EditModeButtonGroup->AddButton(EditModeTileSprite);
	EditGroup->AddChild(EditModeChunk);
	EditGroup->AddChild(EditModeTileType);
	EditGroup->AddChild(EditModeTileSprite);

	MainPanel->AddChild(FileGroup);
	MainPanel->AddChild(EditGroup);

	// Tiles

	// const auto					   ItemView = mWorld->ConstructWidget<PAbstractView>();
	// const std::vector<std::string> Items = {
	// 	"Item1", "Item2", "Item3", "Item4", "Item5"
	// };
	// for (const auto& Item : Items)
	// {
	// 	auto NewItem = ItemView->AddItem<PButton>(Item);
	// 	auto Widget = NewItem->GetWidget<PButton>();
	// 	Widget->SetFixedSize({ 40, 40 });
	// 	Widget->SetResizeMode(RM_Fixed, RM_Fixed);
	// }
	//
	// MainPanel->AddChild(ItemView);

	const auto ClipArea = mWorld->ConstructWidget<PClipArea>();
	MainPanel->AddChild(ClipArea);

	const auto MainCanvas = mWorld->ConstructWidget<PCanvas>();
	MainCanvas->AddChild(MainPanel);

	mWorld->SetRootWidget(MainCanvas);
}

void PEditorGame::AddInputContext(uint8_t InputContext)
{
	mInputContext |= InputContext;

	const auto Input = GetInputManager();
	switch (InputContext)
	{
		case IC_Select:
			mSelectDelegate = Input->KeyUp.AddRaw(this, &PEditorGame::OnKeyUpSelect);
			break;
		case IC_Tile:
			mTileDelegate = Input->KeyUp.AddRaw(this, &PEditorGame::OnKeyUpTile);
			break;
		default:
			break;
	}
}
void PEditorGame::RemoveInputContext(uint8_t InputContext)
{
	mInputContext &= ~InputContext;

	const auto Input = GetInputManager();
	switch (InputContext)
	{
		case IC_Select:
			Input->KeyUp.Remove(mSelectDelegate);
			if (mCurrentChunk)
			{
				mCurrentChunk->SetSelected(false);
			}
			break;
		case IC_Tile:
			Input->KeyUp.Remove(mTileDelegate);
			break;
		default:
			break;
	}
}

void PEditorGame::OnCreateButtonClicked()
{
	LogDebug("Creating new chunk: [{}, {}]", mNewGridSizeX, mNewGridSizeY);

	json JsonData = {
		{ "Position", { 0, 0 }	   },
		{ "SizeX",	   mNewGridSizeX },
		{ "SizeY",	   mNewGridSizeY },
	};
	for (int X = 0; X < mNewGridSizeX; ++X)
	{
		for (int Y = 0; Y < mNewGridSizeY; ++Y)
		{
			JsonData["Tiles"].push_back({
				{ "Position", { X, Y } },
				{ "Type",	  0		},
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
}

void PEditorGame::OnTileButtonChecked(bool State)
{
	RemoveInputContext(IC_Select);
	State ? AddInputContext(IC_Tile) : RemoveInputContext(IC_Tile);
}

void PEditorGame::OnKeyUpTile(uint32_t ScanCode)
{
	const auto HoverTile = GetActorUnderMouse<PTile>();
	if (!HoverTile)
	{
		return;
	}
	switch (ScanCode)
	{
		case SDLK_1:
			HoverTile->Type = TT_Normal;
			break;
		case SDLK_2:
			HoverTile->Type = TT_Obstacle;
			break;
		case SDLK_3:
			HoverTile->Type = TT_Water;
			break;
		case SDLK_4:
			HoverTile->Type = TT_Grass;
			break;
		case SDLK_5:
			HoverTile->Type = TT_Cave;
			break;
		case SDLK_6:
			HoverTile->Type = TT_Portal;
			break;
		default:
			break;
	}
}

void PEditorGame::OnKeyUpSelect(uint32_t ScanCode)
{
	if (!mCurrentChunk)
	{
		return;
	}
	FVector2 Offset;
	switch (ScanCode)
	{
		case SDLK_UP:
			Offset = FVector2(0, TILE_SIZE);
			break;
		case SDLK_DOWN:
			Offset = FVector2(0, -TILE_SIZE);
			break;
		case SDLK_LEFT:
			Offset = FVector2(TILE_SIZE, 0);
			break;
		case SDLK_RIGHT:
			Offset = FVector2(-TILE_SIZE, 0);
			break;
		case SDLK_DELETE:
			// ReSharper disable once CppDFAConstantConditions
			if (mCurrentChunk)
			{
				// Remove the chunk from the list of chunks
				mChunks.erase(std::ranges::remove(mChunks, mCurrentChunk).begin());
				// Destroy the chunk actor
				GetWorld()->DestroyActor(mCurrentChunk);
				// Set the current chunk to null
				mCurrentChunk = nullptr;
			}
			return;
		default:
			break;
	}
	mCurrentChunk->SetPosition(mCurrentChunk->GetPosition() - Offset);
}

void PEditorGame::AddChunk(PChunk* Chunk)
{
	mChunks.emplace_back(Chunk);
	Chunk->Clicked.AddRaw(this, &PEditorGame::ActorSelected);
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
