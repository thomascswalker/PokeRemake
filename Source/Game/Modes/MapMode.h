#pragma once

#include "../FadeTransition.h"
#include "../Interface/TransitionOverlay.h"
#include "Core/PokeParty.h"
#include "Engine/GameMode.h"
#include "Engine/MapManager.h"

#define PLAYER_MAP		"MapName"
#define PLAYER_POSITION "PlayerPosition"

DECLARE_MULTICAST_DELEGATE(DGameMapLoaded);
DECLARE_MULTICAST_DELEGATE(DGameMapUnloaded);

struct SMapContext
{
	std::string PlayerMap;
	std::string PlayerPosition;

	static JSON Schema()
	{
		return {
			{	  PLAYER_MAP, "" },
			{ PLAYER_POSITION, "" }
		};
	}
};

class PMapMode : public PGameMode
{
	PWorld*		 mWorld = nullptr;
	PMapManager* mMapManager = nullptr;
	PGameHUD*	 mHUD = nullptr;

	STimerHandle		mTimerHandle;
	PTransitionOverlay* TransitionOverlay = nullptr;
	STransition*		mFadeTransition = nullptr;

public:
	PMapMode();

	std::string GetName() override;

	bool PreStart() override;

	bool Load() override;
	bool Unload() override;

	void OnGameMapStateChanged(EMapState State);
	void OnFadeInComplete();
	void OnFadeOutComplete();

	bool HandleGameEvent(const SGameEvent& GameEvent) override;
};
