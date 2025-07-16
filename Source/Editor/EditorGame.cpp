#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"
#include "Interface/AbstractView.h"
#include "Interface/Canvas.h"
#include "Interface/Group.h"
#include "Interface/Image.h"
#include "Interface/Panel.h"
#include "Interface/Spinner.h"

static PGroup* TileGroup = nullptr;

std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{ "json", "JSON" },
};

PEditorGame* GetEditorGame()
{
	return dynamic_cast<PEditorGame*>(GetGame());
}

void PEditorGame::PreStart()
{
	GetSettings()->DebugDraw = true;

	const auto EditorView = mWorld->ConstructActor<PEditorView>();
	if (!EditorView)
	{
		LogError("Failed to create Editor View");
	}

	LoadTileset("Tileset1");
	SetupInterface();
}

void PEditorGame::Start()
{
	PGame::Start();
	mWorld->ActorClicked.AddRaw(this, &PEditorGame::OnActorClicked);
}

void PEditorGame::SetupInterface()
{
	const auto MainPanel = mWorld->ConstructWidget<PPanel>();
	MainPanel->SetLayoutMode(LM_Vertical);
	MainPanel->SetResizeModeW(RM_Fixed);
	MainPanel->SetFixedWidth(200);

	// Files

	const auto FileGroup = mWorld->ConstructWidget<PGroup>("File");
	FileGroup->SetResizeModeH(RM_Fit);
	FileGroup->SetLayoutMode(LM_Vertical);

	const auto NewButton = mWorld->ConstructWidget<PButton>("New", this, &PEditorGame::OnNewButtonClicked);
	const auto CreateButton = mWorld->ConstructWidget<PButton>("Create", this, &PEditorGame::OnCreateButtonClicked);
	const auto SizeXSpinner = mWorld->ConstructWidget<PSpinner>(mNewGridSizeX);
	SizeXSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeXChanged);
	const auto SizeYSpinner = mWorld->ConstructWidget<PSpinner>(mNewGridSizeY);
	SizeYSpinner->ValueChanged.AddRaw(this, &PEditorGame::OnSizeYChanged);
	const auto SaveButton = mWorld->ConstructWidget<PButton>("Save", this, &PEditorGame::OnSaveButtonClicked);
	SaveButton->SetFontSize(WIDGET_FONT_SIZE);
	const auto LoadButton = mWorld->ConstructWidget<PButton>("Load", this, &PEditorGame::OnLoadButtonClicked);
	LoadButton->SetFontSize(WIDGET_FONT_SIZE);
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

	MainPanel->AddChild(FileGroup);
	MainPanel->AddChild(EditGroup);

	// Tiles

	PAbstractView* ItemView = mWorld->ConstructWidget<PAbstractView>();
	ItemView->SetVisible(true);
	const auto ItemViewButtonGroup = mWorld->ConstructWidget<PButtonGroup>();

	auto Tileset = GetTileset("Tileset1");
	auto TilesetTexture = Tileset->Texture;

	for (auto& Item : Tileset->Items)
	{
		auto NewItem = ItemView->AddItem<PButton>();
		auto Button = NewItem->GetWidget<PButton>();
		Button->SetResizeMode(RM_Fixed, RM_Fixed);
		Button->SetFixedSize(32, 32);
		Button->SetCheckable(true);
		Button->SetCustomData(&Item);
		Button->Checked.AddRaw(this, &PEditorGame::OnTilesetButtonChecked);

		ItemViewButtonGroup->AddButton(Button);

		// 16x6
		PImage* Img = ItemView->AddItem<PImage>(TilesetTexture)->GetWidget<PImage>();
		Button->AddChild(Img);
		Img->SetFixedSize({ 30, 30 });
		Img->SetResizeMode(RM_Fixed, RM_Fixed);
		Img->SetUseSourceRect(true);

		FRect SourceRect = Item.GetSourceRect();
		Img->SetSourceRect(SourceRect);
	}

	TileGroup = mWorld->ConstructWidget<PGroup>("Tiles");
	TileGroup->SetLayoutMode(LM_Vertical);
	TileGroup->SetVisible(false);
	TileGroup->AddChild(ItemView);

	MainPanel->AddChild(TileGroup);

	const auto MainCanvas = mWorld->ConstructWidget<PCanvas>();
	MainCanvas->AddChild(MainPanel);

	mWorld->SetRootWidget(MainCanvas);
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
				mBrushSize = std::clamp(
					Event->KeyUp == SDLK_UP
						? mBrushSize + 1
						: mBrushSize - 1,
					1,
					3);
				;
			}
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
		{ "Tileset",	 "Tileset1"	}
	};
	for (int X = 0; X < mNewGridSizeX; ++X)
	{
		for (int Y = 0; Y < mNewGridSizeY; ++Y)
		{
			JsonData["Tiles"].push_back({
				{ "Position", { X, Y }   },
				{ "Tileset",	 "Tileset1" },
				{ "Index",	   0			 }
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
	if (State)
	{
		mCurrentTilesetItem = Sender->GetCustomData<STile>();
	}
	else
	{
		mCurrentTilesetItem = nullptr;
	}
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
		if (auto Tile = dynamic_cast<PTile*>(ClickedActor))
		{
			Tile->TilesetIndex = mCurrentTilesetItem->Index;
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
