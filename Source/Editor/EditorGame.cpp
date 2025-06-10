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
}

void PEditorGame::Start()
{
	LogDebug("Starting {}", GetInternalName().c_str());
	PGame::Start();
}
