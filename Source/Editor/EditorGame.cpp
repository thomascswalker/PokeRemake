#include "EditorGame.h"

#include "Core/Logging.h"
#include "EditorView.h"

void PEditorGame::PreStart()
{
	SetInternalName("EditorGame");
	GetSettings()->bDebugDraw = true;

	if (const auto W = GetWorld())
	{
		auto EV = W->ConstructActor<PEditorView>();
		if (!EV)
		{
			LogError("Failed to create Editor View");
		}
		EV->SetInternalName("EditorView");
	}

	ConstructInterface();
}

void PEditorGame::Start()
{
	LogDebug("Starting {}", GetInternalName().c_str());
	PGame::Start();
}

void PEditorGame::ConstructInterface()
{
	if (const auto W = GetWorld())
	{
		mNewButton = W->ConstructWidget<PButton>("New");
		mNewButton->W = BUTTON_WIDTH;
		mNewButton->H = BUTTON_HEIGHT;

		mEditButton = W->ConstructWidget<PButton>("Edit");
		mEditButton->W = BUTTON_WIDTH;
		mEditButton->H = BUTTON_HEIGHT;
		mEditButton->Clicked.AddRaw(this, &PEditorGame::OnEditButtonClicked);

		mSaveButton = W->ConstructWidget<PButton>("Save");
		mSaveButton->W = BUTTON_WIDTH;
		mSaveButton->H = BUTTON_HEIGHT;

		mModeText = W->ConstructWidget<PText>("View", 20);
		mModeText->W = BUTTON_WIDTH;
		mModeText->H = BUTTON_HEIGHT;

		mCanvas = W->ConstructWidget<PCanvas>();
		mCanvas->X = 10;
		mCanvas->Y = 10;
		mCanvas->AddChild(mNewButton);
		mCanvas->AddChild(mEditButton);
		mCanvas->AddChild(mSaveButton);
		mCanvas->AddChild(mModeText);

		W->SetCanvas(mCanvas);
	}
}

void PEditorGame::OnEditButtonClicked()
{
	bEditMode = !bEditMode;
	const auto Text = bEditMode ? "Edit" : "View";
	mModeText->SetText(Text);
}
