#pragma once
#include "Engine/World.h"
#include "Interface/HUD.h"

#include "BattleManager.h"
#include "BattleView.h"

class PBattleHUD : public PHUD
{
public:
	PBattleHUD()
	{
		auto Mgr = PBattleManager::Instance();

		auto PlayerMon = Mgr->GetPlayerPokemon();
		auto BattleMon = Mgr->GetBattlePokemon();

		auto PlayerSprite = ConstructWidget<PBattleSprite>(PlayerMon, true);
		auto BattleSprite = ConstructWidget<PBattleSprite>(BattleMon, false);

		PWidget::AddChild(PlayerSprite);
		PWidget::AddChild(BattleSprite);
	}
};