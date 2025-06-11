#include "EditorGame.h"

#include "Core/Logging.h"
#include "EditorView.h"

#include <stdio.h>
#include <stdlib.h>

constexpr int BUTTON_WIDTH = 60;
constexpr int BUTTON_HEIGHT = 20;

static void OnClicked()
{
	LogDebug("Click!");
}

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

void PEditorGame::ConstructInterface() const
{
	if (const auto W = GetWorld())
	{
		auto Button1 = W->ConstructWidget<PButton>("Edit");
		Button1->W = BUTTON_WIDTH;
		Button1->H = BUTTON_HEIGHT;
		Button1->Clicked.AddStatic(&OnClicked);

		auto Button2 = W->ConstructWidget<PButton>("Save");
		Button2->W = BUTTON_WIDTH;
		Button2->H = BUTTON_HEIGHT;
		Button2->Clicked.AddStatic(&OnClicked);

		auto Layout = W->ConstructWidget<PVerticalLayout>();
		Layout->X = 10;
		Layout->Y = 10;
		Layout->AddChild(Button1);
		Layout->AddChild(Button2);

		W->SetRootWidget(Layout);
	}
}
