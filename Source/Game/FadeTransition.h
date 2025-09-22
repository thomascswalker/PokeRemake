#pragma once

#include "Engine/Transition.h"
#include "Interface/Game/GameHUD.h"
#include "Interface/TransitionOverlay.h"

struct SFadeTransition : STransition
{
	PTransitionOverlay* Overlay;

	void Start() override
	{
		LogInfo("Fade Transition Started");

		Duration = 100.0f;

		Overlay = GetWorld()->ConstructWidget<PTransitionOverlay>(this);
		if (!Overlay)
		{
			LogError("Failed to construct fade transition overlay.");
			return;
		}
		auto HUD = GetHUD<PGameHUD>();
		HUD->AddChild(Overlay);

		// Overlay->SetOpacity(0.0f);
		Overlay->StartFade(FM_Out);
		Overlay->FadedOut.AddRaw(this, &SFadeTransition::End);
		STransition::Start();
	}

	void End() override
	{
		LogInfo("Fade Transition Ended");

		if (!Overlay)
		{
			LogError("Fade transition overlay is invalid.");
			return;
		}
		Overlay->Unparent();
		GetWorld()->DestroyWidget(Overlay);

		STransition::End();
	}
};