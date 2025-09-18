#pragma once

#include "Engine/Timer.h"
#include "Engine/World.h"
#include "Interface/Widget.h"

DECLARE_MULTICAST_DELEGATE(DFadedOut);
DECLARE_MULTICAST_DELEGATE(DFadedIn);

enum EFadeMode
{
	FM_In,
	FM_Out
};

constexpr float TRANSITION_DURATION = 1000.0f;

class PTransitionOverlay : public PWidget
{
	STimerHandle mTimerHandle;
	float		 Opacity = 1.0f;
	float		 Duration = TRANSITION_DURATION; // 1 second
	float		 Rate = Duration / 100.0f;		 // 100 increments of the fade
	bool		 mFadeOut = true;

	void FadeOut()
	{
		if (!mFadeOut)
		{
			return;
		}
		Opacity -= Rate;
		if (Opacity <= 0.0f)
		{
			FadedOut.Broadcast();
			mFadeOut = false;
		}
	}

	void FadeIn()
	{
		if (mFadeOut)
		{
			return;
		}
		Opacity += Rate;
		if (Opacity >= 1.0f)
		{
			FadedIn.Broadcast();
			Unparent();
			GetWorld()->DestroyWidget(this);
		}
	}

public:
	DFadedOut FadedOut;
	DFadedIn  FadedIn;

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(0, 0, 0, 255);
		Renderer->DrawFillRect({ 0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT });
	}

	void StartFade(EFadeMode Mode)
	{
		GetWorld()->GetTimerManager()->ClearTimer(mTimerHandle);
		switch (Mode)
		{
			case FM_In:
				Opacity = 0.0f;
				GetWorld()->GetTimerManager()->SetTimer(mTimerHandle, this, &PTransitionOverlay::FadeIn, Rate, true);
				break;
			case FM_Out:
				Opacity = 1.0f;
				GetWorld()->GetTimerManager()->SetTimer(mTimerHandle, this, &PTransitionOverlay::FadeOut, Rate, true);
				break;
		}
	}
};