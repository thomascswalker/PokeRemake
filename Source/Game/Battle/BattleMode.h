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

	bool HandlePressA() override;
	bool HandlePressB() override;
	bool HandlePressDPad(EDPad Direction) override;

	void EndBattle() const;
	void EnterMoveSelection();
	void ExitMoveSelection();
	void DealDamage(SPokemonMove* Move, const SPokemon* Attacker, SPokemon* Target) const;

	void ExecuteTurn() const;
	void EndTurn() const;

	void ChangeActionSelection(uint8_t Direction);
	void ChangeMoveSelection(EDPad Direction);

	void StartDialog(const SDialogContext& Context) override {}
};
