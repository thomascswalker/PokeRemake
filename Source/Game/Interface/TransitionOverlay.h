#pragma once

#include "Engine/Timer.h"
#include "Interface/Widget.h"

DECLARE_MULTICAST_DELEGATE(DFadedOut);
DECLARE_MULTICAST_DELEGATE(DFadedIn);

enum EFadeMode
{
	FM_In,
	FM_Out,
};

class PTransitionOverlay : public PWidget
{
	STimerHandle Handle;
	float		 mOpacity = 0.0f;
	float		 mElapsed = 0.0f;
	EFadeMode	 mMode = FM_Out;

public:
	DFadedOut FadedOut;
	DFadedIn  FadedIn;

	void Draw(const PRenderer* Renderer) const override
	{
		const auto Alpha = static_cast<int32_t>(255.0f * mOpacity);
		Renderer->SetDrawColor(0, 0, 0, Alpha);
		Renderer->DrawFillRect({ 0, 0, WINDOW_PXL_WIDTH, WINDOW_PXL_HEIGHT });
	}

	void Tick(float DeltaTime) override
	{
		// Increment elapsed time
		mElapsed += DeltaTime;

		// Percentage of time elapsed
		float DeltaOpacity = DeltaTime / TRANSITION_DURATION;

		switch (mMode)
		{
			case FM_In:
				mOpacity -= DeltaOpacity;
				if (mOpacity <= 0.0f)
				{
					FadedIn.Broadcast();
				}
				break;
			case FM_Out:
				mOpacity += DeltaOpacity;
				if (mOpacity >= 1.0f)
				{
					FadedOut.Broadcast();
				}
				break;
			default:
				break;
		}
	}

	void Fade(EFadeMode Mode)
	{
		mElapsed = 0;
		mMode = Mode;
		switch (Mode)
		{
			case FM_In:
				mOpacity = 1.0f;
				break;
			case FM_Out:
				mOpacity = 0.0f;
				break;
		}
	}

	float GetOpacity() const { return mOpacity; }
	void  SetOpacity(float Opacity) { mOpacity = Opacity; }
};