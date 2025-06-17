#include "EditorGame.h"

#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"
#include "Interface/Box.h"
#include "Interface/Spinner.h"

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
	InitializeControls();
}

void PEditorGame::Start()
{
	mWorld->Start();
	FindActiveCamera();
}

void PEditorGame::ConstructInterface()
{
	mCreateButton = mWorld->ConstructWidget<PButton>("Create");
	mCreateButton->Clicked.AddRaw(this, &PEditorGame::OnCreateButtonClicked);
	mCreateButton->SetFontSize(WIDGET_FONT_SIZE);
	mCreateButton->SetResizeMode(RM_ExpandX);

	auto Box = mWorld->ConstructWidget<PBox>();
	auto SizeXSpinner = mWorld->ConstructWidget<PSpinner>();
	auto SizeYSpinner = mWorld->ConstructWidget<PSpinner>();
	Box->SetResizeMode(RM_ExpandY);
	Box->SetLayoutMode(LM_Horizontal);
	Box->AddChild(SizeXSpinner);
	Box->AddChild(SizeYSpinner);

	mSaveButton = mWorld->ConstructWidget<PButton>("Save");
	mSaveButton->SetFontSize(WIDGET_FONT_SIZE);
	mSaveButton->Clicked.AddRaw(this, &PEditorGame::OnSaveButtonClicked);
	mSaveButton->SetResizeMode(RM_ExpandX);

	mLoadButton = mWorld->ConstructWidget<PButton>("Load");
	mLoadButton->SetFontSize(WIDGET_FONT_SIZE);
	mLoadButton->Clicked.AddRaw(this, &PEditorGame::OnLoadButtonClicked);
	mLoadButton->SetResizeMode(RM_ExpandX);

	mModeText = mWorld->ConstructWidget<PText>("View");
	mModeText->SetFontSize(WIDGET_FONT_SIZE);

	mCanvas = mWorld->ConstructWidget<PCanvas>();
	mCanvas->AddChild(mCreateButton);
	mCanvas->AddChild(Box);
	mCanvas->AddChild(mSaveButton);
	mCanvas->AddChild(mLoadButton);
	mCanvas->AddChild(mModeText);

	mWorld->SetCanvas(mCanvas);
}

void PEditorGame::InitializeControls()
{
	if (auto Input = GetInputManager())
	{
		Input->KeyUp.AddRaw(this, &PEditorGame::OnKeyUp);
	}
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

void PEditorGame::OnKeyUp(uint32_t ScanCode)
{
	switch (ScanCode)
	{
		case SDLK_UP:
			{
				if (mCurrentChunk)
				{
					mCurrentChunk->SetPosition(mCurrentChunk->GetPosition()
											   - FVector2(0, TILE_SIZE));
				}
				break;
			}
		case SDLK_DOWN:
			{
				if (mCurrentChunk)
				{
					mCurrentChunk->SetPosition(mCurrentChunk->GetPosition()
											   + FVector2(0, TILE_SIZE));
				}
				break;
			}
		case SDLK_LEFT:
			{
				if (mCurrentChunk)
				{
					mCurrentChunk->SetPosition(mCurrentChunk->GetPosition()
											   - FVector2(TILE_SIZE, 0));
				}
				break;
			}
		case SDLK_RIGHT:
			{
				if (mCurrentChunk)
				{
					mCurrentChunk->SetPosition(mCurrentChunk->GetPosition()
											   + FVector2(TILE_SIZE, 0));
				}
				break;
			}
		default:
			break;
	}
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
	if (auto Chunk = dynamic_cast<PChunk*>(Actor))
	{
		// Deselect all other chunks
		for (auto C : mWorld->GetActorsOfType<PChunk>())
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
