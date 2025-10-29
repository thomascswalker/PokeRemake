#pragma once
#include "Engine/Game.h"
#include "Engine/GameEvent.h"

#include "BattleManager.h"
#include "BattleMode.h"

inline uint32_t ComputeDamage(SPokemonMove* Move, const SPokemon* Attacker, const SPokemon* Target)
{
	// https://bulbapedia.bulbagarden.net/wiki/Damage#Generation_I

	// Compute base damage
	uint32_t LevelFactor = 2 * Attacker->GetLevel() / 5 + 2;
	uint32_t PowerFactor = Move->GetDef()->Power;
	uint32_t StatFactor = Attacker->GetAttack() / Target->GetDefense();
	uint32_t BaseDamage = LevelFactor * PowerFactor * StatFactor / 50 + 2;

	// Compute same type effectiveness (STAT). This is 1.5 if the Move is any of the Target's types.
	// It is 1 otherwise.
	float SameTypeEffectiveness = Target->IsAnyType(Move->GetDef()->Type) ? 1.5f : 1.0f;

	// TODO: Add type effectiveness matrix.

	return BaseDamage * SameTypeEffectiveness;
}

struct SUseMoveEvent : SGameEvent
{
	SPokemonMove* Move = nullptr;
	SPokemon*	  Attacker = nullptr;
	SPokemon*	  Target = nullptr;

	[[nodiscard]] SUseMoveEvent(SPokemonMove* const move, SPokemon* const attacker, SPokemon* const target)
		: Move(move),
		  Attacker(attacker),
		  Target(target) {}

	void Execute() const override
	{
		if (Move->GetPP() == 0)
		{
			return;
		}
		// TODO: Handle non-power (status, healing, etc.) moves.

		LogInfo("{} used {}", Attacker->GetDisplayName().c_str(), Move->GetDef()->Name.c_str());

		uint32_t DamageDealt = ComputeDamage(Move, Attacker, Target);
		LogInfo("{} damage dealt.", DamageDealt);

		uint32_t RemainingHp = Target->Damage(DamageDealt);

		LogInfo("{} has {} HP remaining.", Target->GetDisplayName().c_str(), RemainingHp);
	}
};