#include "EditorGame.h"

#include "Core/Logging.h"
#include "EditorView.h"

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

void PEditorGame::Tick(float DeltaTime)
{
	PGame::Tick(DeltaTime);
	auto A = GetRenderer()->GetActorAtUnderMouse();
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

void PEditorGame::AddChunk()
{
	int		   DefaultChunkSize = 10;
	SChunkData Data = {
		{ 0, 0, DefaultChunkSize, DefaultChunkSize }
	};

	// Fill all tiles with 0
	for (int i = 0; i < Data.Geometry.H; ++i)
	{
		Data.Data.emplace_back(std::vector(Data.Geometry.W, 0));
	}

	mWorld->SpawnActor<PChunk>(Data);
}
