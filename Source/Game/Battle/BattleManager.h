#pragma once

#include "../Core/Pokemon.h"
#include "../Core/PokeParty.h"
#include "../Core/PokeStorage.h"
#include "Engine/Object.h"

struct STrainer
{
	int32_t			Id;
	std::string		Name;
	PPokemonStorage Storage;
};

struct SBattleContext
{
	// Pointer to the trainer definition
	STrainer* Trainer = nullptr;

	// The opposing party
	PPokemonParty BattleParty;

	// Pointer to the player's current Pokemon
	SPokemon* PlayerMon = nullptr;

	// Pointer to the opponent's current Pokemon
	SPokemon* BattleMon = nullptr;

	static JSON Schema()
	{
		return {
			{  "BattleId", nullptr },
			{ "PlayerMon", nullptr },
			{ "BattleMon", nullptr }
		};
	}
};

class PBattleManager : public PObject
{
	std::map<int32_t, STrainer> mTrainers;
	SBattleContext				mContext;

public:
	bool PreStart() override;

	/* Any battle */

	PPokemonParty* GetCurrentBattleParty() { return &mContext.BattleParty; }
	EPartyType	   GetCurrentBattlePartyType() const { return mContext.BattleParty.GetType(); }

	void StartTrainerBattle(int32_t Id);
	void StartWildBattle(int32_t Id, int32_t Level);

	/* Trainers */

	STrainer* GetTrainer(int32_t Id);
	int32_t	  GetCurrentTrainerId() const { return mContext.Trainer->Id; }

	PPokemonStorage* GetCurrentTrainerStorage(int32_t Id);
	std::string		 GetCurrentTrainerName() const;

	SPokemon* GetPlayerMon() const
	{
		return mContext.PlayerMon;
	}

	void SetPlayerMon(SPokemon* PlayerMon)
	{
		mContext.PlayerMon = PlayerMon;
	}

	SPokemon* GetBattleMon() const
	{
		return mContext.BattleMon;
	}

	void SetBattleMon(SPokemon* BattleMon)
	{
		mContext.BattleMon = BattleMon;
	}

	void SetBattleMon(int32_t Index)
	{
		mContext.BattleMon = mContext.BattleParty.Get(Index);
	}

	void SwapNextBattleMon();
	void SwapPrevBattleMon();
};

extern PBattleManager* GBattleManager;