#pragma once

#include "Engine/GameMode.h"
#include "Engine/MapManager.h"
#include "Interface/TransitionOverlay.h"

#include "FadeTransition.h"

DECLARE_MULTICAST_DELEGATE(DGameMapLoaded);
DECLARE_MULTICAST_DELEGATE(DGameMapUnloaded);

class PMapMode : public PGameMode
{
	PWorld* mWorld = nullptr;

	STimerHandle		mTimerHandle;
	PTransitionOverlay* TransitionOverlay = nullptr;
	STransition*		mFadeTransition = nullptr;

public:
	PMapMode();

	std::string GetName() override
	{
		return "MapMode";
	}

	bool Load() override;
	bool Unload() override;
	void OnGameMapLoaded();
	void OnGameMapUnloaded();

	void OnKeyUp(SInputEvent* Event) override;
};
