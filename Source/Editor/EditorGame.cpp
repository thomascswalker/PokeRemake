#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"

#define NEW_GRID_SIZE 5

std::vector<std::pair<std::string, std::string>> gDefaultFilters = {
	{ "json", "JSON" },
};

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
	mCreateButton = mWorld->ConstructWidget<PButton>("Create");
	mCreateButton->W = BUTTON_WIDTH;
	mCreateButton->H = BUTTON_HEIGHT;
	mCreateButton->Clicked.AddRaw(this, &PEditorGame::OnCreateButtonClicked);
	mCreateButton->SetFontSize(WIDGET_FONT_SIZE);

	mSaveButton = mWorld->ConstructWidget<PButton>("Save");
	mSaveButton->W = BUTTON_WIDTH;
	mSaveButton->H = BUTTON_HEIGHT;
	mSaveButton->SetFontSize(WIDGET_FONT_SIZE);
	mSaveButton->Clicked.AddRaw(this, &PEditorGame::OnSaveButtonClicked);

	mLoadButton = mWorld->ConstructWidget<PButton>("Load");
	mLoadButton->W = BUTTON_WIDTH;
	mLoadButton->H = BUTTON_HEIGHT;
	mLoadButton->SetFontSize(WIDGET_FONT_SIZE);
	mLoadButton->Clicked.AddRaw(this, &PEditorGame::OnLoadButtonClicked);

	mModeText = mWorld->ConstructWidget<PText>("View");
	mModeText->W = BUTTON_WIDTH;
	mModeText->H = BUTTON_HEIGHT;
	mModeText->SetFontSize(WIDGET_FONT_SIZE);

	mCanvas = mWorld->ConstructWidget<PCanvas>();
	mCanvas->X = 10;
	mCanvas->Y = 10;
	mCanvas->AddChild(mCreateButton);
	mCanvas->AddChild(mSaveButton);
	mCanvas->AddChild(mLoadButton);
	mCanvas->AddChild(mModeText);

	mWorld->SetCanvas(mCanvas);
}

void PEditorGame::OnCreateButtonClicked()
{
	json JsonData = {
		{ "Position", { 0, 0 }	   },
		{ "SizeX",	   NEW_GRID_SIZE },
		{ "SizeY",	   NEW_GRID_SIZE },
	};
	for (int X = 0; X < NEW_GRID_SIZE; ++X)
	{
		for (int Y = 0; Y < NEW_GRID_SIZE; ++Y)
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

	for (auto Actor : mWorld->GetActors())
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

	json JsonData = json::parse(Data.data());
	Serializer.Deserialize(JsonData);
}

void PEditorGame::AddChunk(PChunk* Chunk)
{
	mChunks.emplace_back(Chunk);
	SetCurrentChunk(Chunk);
}

void PEditorGame::SetCurrentChunk(PChunk* Chunk)
{
	if (mCurrentChunk)
	{
		for (const auto& Tile : mCurrentChunk->GetTiles())
		{
			Tile->Clicked.RemoveAll();
		}
	}

	mCurrentChunk = Chunk;

	// Bind all tile clicks to SelectTile
	for (const auto& Tile : mCurrentChunk->GetTiles())
	{
		Tile->Clicked.AddRaw(this, &PEditorGame::SelectTile);
	}
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

void PEditorGame::DeselectAllTiles()
{
	for (const auto& Chunk : mChunks)
	{
		for (const auto& Tile : Chunk->GetTiles())
		{
			Tile->SetSelected(false);
		}
	}
}

void PEditorGame::SelectTile(PActor* Actor)
{
	const auto Tile = static_cast<PTile*>(Actor);

	// Deselect tiles
	if (GetInputManager()->IsCtrlDown())
	{
		Tile->SetSelected(false);
		return;
	}

	// If shift is not pressed, deselect all tiles
	if (!GetInputManager()->IsShiftDown())
	{
		DeselectAllTiles();
	}

	// Select the clicked tile. If shift is pressed, add tile to selection
	Tile->SetSelected(true);
}
