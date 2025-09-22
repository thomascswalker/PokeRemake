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
	STransition* mTransition;
	STimerHandle Handle;
	float		 mOpacity = 1.0f;
	float		 mDuration = TRANSITION_DURATION; // 1 second
	float		 mRate = mDuration / 10000.0f;	  // 100 increments of the fade
	bool		 mFadeOut = true;

	void FadeOut()
	{
		mOpacity -= mRate;
		if (mOpacity <= 0.0f)
		{
			FadedOut.Broadcast();
			mFadeOut = false;
		}
	}

	void FadeIn()
	{
		mOpacity += mRate;
		if (mOpacity >= 1.0f)
		{
			FadedIn.Broadcast();
			Unparent();
			GetWorld()->DestroyWidget(this);
		}
	}

public:
	DFadedOut FadedOut;
	DFadedIn  FadedIn;

	PTransitionOverlay(STransition* Transition)
	{
		mTransition = Transition;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(0, 0, 0, static_cast<uint8_t>(255.0f * mOpacity));
		Renderer->DrawFillRect({ 0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT });
	}

	void StartFade(EFadeMode Mode)
	{
		auto Mgr = GetWorld()->GetTimerManager();
		Mgr->ClearTimer(Handle);
		switch (Mode)
		{
			case FM_In:
				mOpacity = 0.0f;
				Mgr->SetTimer(Handle, this, &PTransitionOverlay::FadeIn, mRate, true);
				break;
			case FM_Out:
				mOpacity = 1.0f;
				Mgr->SetTimer(Handle, this, &PTransitionOverlay::FadeOut, mRate, true);
				break;
		}
	}

	float GetOpacity() const { return mOpacity; }
	void  SetOpacity(float Opacity) { mOpacity = Opacity; }
};