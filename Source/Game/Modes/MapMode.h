#pragma once

#include "../FadeTransition.h"
#include "../Interface/TransitionOverlay.h"
#include "Engine/GameMode.h"
#include "Engine/MapManager.h"

DECLARE_MULTICAST_DELEGATE(DGameMapLoaded);
DECLARE_MULTICAST_DELEGATE(DGameMapUnloaded);

class PMapMode : public PGameMode
{
	PWorld*		 mWorld = nullptr;
	PMapManager* mMapManager = nullptr;

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

	void OnGameMapStateChanged(EMapState State);
	void OnFadeInComplete();
	void OnFadeOutComplete();

	void OnKeyUp(SInputEvent* Event) override;
};
