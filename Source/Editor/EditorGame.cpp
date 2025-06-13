#include "EditorGame.h"

#include "Core/Logging.h"
#include "EditorView.h"
#include "Engine/InputManager.h"
#include "Engine/Serializer.h"

void PEditorGame::PreStart()
{
	GetSettings()->bDebugDraw = true;

	const auto EV = mWorld->ConstructActor<PEditorView>();
	if (!EV)
	{
		LogError("Failed to create Editor View");
	}

	ConstructInterface();
	if (const auto Input = GetInputManager())
	{
		Input->MouseRightClick.AddRaw(this, &PEditorGame::DeselectAllTiles);
	}
}

void PEditorGame::Start()
{
	mWorld->Start();
	FindActiveCamera();
}

void PEditorGame::ConstructInterface()
{
	mNewButton = mWorld->ConstructWidget<PButton>("New");
	mNewButton->W = BUTTON_WIDTH;
	mNewButton->H = BUTTON_HEIGHT;
	mNewButton->Clicked.AddRaw(this, &PEditorGame::AddChunk);
	mNewButton->SetFontSize(WIDGET_FONT_SIZE);

	mEditButton = mWorld->ConstructWidget<PButton>("Edit");
	mEditButton->W = BUTTON_WIDTH;
	mEditButton->H = BUTTON_HEIGHT;
	mEditButton->SetFontSize(WIDGET_FONT_SIZE);
	mEditButton->Clicked.AddRaw(this, &PEditorGame::OnEditButtonClicked);

	mSaveButton = mWorld->ConstructWidget<PButton>("Save");
	mSaveButton->W = BUTTON_WIDTH;
	mSaveButton->H = BUTTON_HEIGHT;
	mSaveButton->SetFontSize(WIDGET_FONT_SIZE);
	mSaveButton->Clicked.AddRaw(this, &PEditorGame::OnSaveButtonClicked);

	mModeText = mWorld->ConstructWidget<PText>("View");
	mModeText->W = BUTTON_WIDTH;
	mModeText->H = BUTTON_HEIGHT;
	mModeText->SetFontSize(WIDGET_FONT_SIZE);

	mCanvas = mWorld->ConstructWidget<PCanvas>();
	mCanvas->X = 10;
	mCanvas->Y = 10;
	mCanvas->AddChild(mNewButton);
	mCanvas->AddChild(mEditButton);
	mCanvas->AddChild(mSaveButton);
	mCanvas->AddChild(mModeText);

	mWorld->SetCanvas(mCanvas);
}

void PEditorGame::OnEditButtonClicked()
{
	bEditMode = !bEditMode;
	const auto Text = bEditMode ? "Edit" : "View";
	mModeText->SetText(Text);
}

void PEditorGame::OnSaveButtonClicked()
{
	PSerializer Serializer;

	for (auto Actor : mWorld->GetActors())
	{
		Serializer.Serialize(Actor);
	}

	LogDebug("Serialized: {}", Serializer.GetSerializedData().dump(4));
}

void PEditorGame::AddChunk()
{
	int		   DefaultChunkSize = 10;
	SChunkData Data = {
		{
			0, 0,
			DefaultChunkSize, DefaultChunkSize,
		 },
	};

	// Fill all tiles with 0
	for (int i = 0; i < Data.Geometry.H; ++i)
	{
		Data.Data.emplace_back(std::vector(Data.Geometry.W, 0));
	}

	// Create the chunk
	const auto Chunk = mWorld->SpawnActor<PChunk>(Data);
	if (!Chunk)
	{
		LogError("Failed to create chunk");
		return;
	}
	mChunks.emplace_back(Chunk);

	// Bind all tile clicks to SelectTile
	for (const auto& Tile : Chunk->GetTiles())
	{
		Tile->Clicked.AddRaw(this, &PEditorGame::SelectTile);
	}
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
