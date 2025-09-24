#pragma once

#include "Battle/BattleHUD.h"
#include "Battle/BattleManager.h"
#include "Battle/BattleView.h"
#include "Engine/GameMode.h"

class PBattleMode : public PGameMode
{
	PBattleView* mBattleView = nullptr;
	PBattleHUD*	 mBattleHUD = nullptr;

public:
	PBattleMode();
	std::string GetName() override
	{
		return "BattleMode";
	}

	bool Load() override;
	bool Unload() override;

	void OnKeyUp(SInputEvent* Event) override;

	void StartBattle(const SBattleEvent& Event);
	void EndBattle();
};
