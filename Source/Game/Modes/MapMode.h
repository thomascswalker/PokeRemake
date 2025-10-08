#pragma once

#include "../Interface/GameHUD.h"
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
	PGameHUD*		  mHUD = nullptr;
	PPlayerCharacter* mPlayerCharacter = nullptr;

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

	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;

	bool HandlePressA() override;
	bool HandleReleaseA() override;
	bool HandlePressB() override;
	bool HandleReleaseB() override;
	bool HandlePressDPad(EDPad Direction) override;
	bool HandleReleaseDPad(EDPad Direction) override;
	bool HandleGameEvent(const SGameEvent& GameEvent) override;
};
