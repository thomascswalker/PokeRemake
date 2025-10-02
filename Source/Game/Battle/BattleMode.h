#pragma once

#include "Engine/GameMode.h"
#include "Interface/GameHUD.h"

#include "BattleManager.h"

#define STATE_BATTLE_ID "BattleId"

class PBattleMode : public PGameMode
{
	PBattleManager mBattleManager;
	PGameHUD*	   mHUD = nullptr;

public:
	PBattleMode();

	bool		PreStart() override;
	bool		Start() override;
	bool		Load() override;
	bool		Unload() override;
	std::string GetName() override;

	void OnKeyUp(SInputEvent* Event) override;

	bool HandleGameEvent(SGameEvent& GameEvent) override;
};
