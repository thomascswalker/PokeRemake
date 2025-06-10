#include "EditorGame.h"

#include "Core/Logging.h"
#include "EditorView.h"

#include <stdio.h>
#include <stdlib.h>

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
		auto Button1 = W->ConstructWidget<PButton>("My Button 1");
		Button1->W = 200;
		Button1->H = 50;
		Button1->Clicked.AddStatic(&OnClicked);

		auto Button2 = W->ConstructWidget<PButton>("My Button 2");
		Button2->W = 200;
		Button2->H = 50;
		Button2->Clicked.AddStatic(&OnClicked);

		auto Layout = W->ConstructWidget<PVerticalLayout>();
		Layout->X = 10;
		Layout->Y = 10;
		Layout->AddChild(Button1);
		Layout->AddChild(Button2);

		W->SetRootWidget(Layout);
	}
}
