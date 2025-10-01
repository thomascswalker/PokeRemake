#pragma once

#include "../Core/Pokemon.h"
#include "../Core/PokeParty.h"
#include "../Core/PokeStorage.h"
#include "Engine/Object.h"

#define BATTLE_ID_GARY_OAK_LAB 1

struct SBattleContext
{
	SPokemon* PlayerMon = nullptr;
	SPokemon* BattleMon = nullptr;
	uint32_t  BattleId = 0;

	static JSON Schema()
	{
		return {
			{  "BattleId",		  0 },
			{ "PlayerMon", nullptr },
			{ "BattleMon", nullptr }
		};
	}
};

class PBattleManager : public PObject
{
	std::map<int32_t, PPokemonStorage> mBattles;
	PPokemonParty					   mBattleParty;
	SBattleContext					   mContext;

public:
	bool Start() override;

	int32_t GetBattleId() const { return mContext.BattleId; }
	void	SetBattleId(int32_t Id)
	{
		mContext.BattleId = Id;
		if (mBattles.contains(Id))
		{
			mBattleParty.Clear();
			for (auto& Mon : mBattles.at(Id).GetAll())
			{
				mBattleParty.Add(&Mon);
			}
		}
		mContext.BattleMon = mBattleParty.Get(0);
	}

	PPokemonStorage* GetBattleStorage(int32_t Id)
	{
		if (!mBattles.contains(Id))
		{
			return nullptr;
		}
		return &mBattles[Id];
	}

	PPokemonParty* GetBattleParty() { return &mBattleParty; }

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
};

extern PBattleManager* GBattleManager;