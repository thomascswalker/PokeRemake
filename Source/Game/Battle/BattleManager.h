#pragma once

#include "../Core/Pokemon.h"
#include "../Core/PokeParty.h"
#include "../Core/PokeStorage.h"
#include "Engine/Object.h"

struct SBattle
{
	int32_t			Id;
	std::string		Name;
	PPokemonStorage Storage;
};

struct SBattleContext
{
	// Pointer to the battle definition
	SBattle* Battle = nullptr;

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
	std::map<int32_t, SBattle> mBattles;
	PPokemonParty			   mBattleParty;
	SBattleContext			   mContext;

public:
	bool PreStart() override;

	SBattle* GetBattle(int32_t Id);

	int32_t GetCurrentBattleId() const { return mContext.Battle->Id; }
	void	SetCurrentBattleId(int32_t Id);

	PPokemonStorage* GetCurrentBattleStorage(int32_t Id);
	PPokemonParty*	 GetCurrentBattleParty() { return &mBattleParty; }
	std::string		 GetCurrentBattleName() const;

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
		mContext.BattleMon = mBattleParty.Get(Index);
	}

	void NextBattleMon();
};

extern PBattleManager* GBattleManager;