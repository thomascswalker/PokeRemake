#pragma once

#include "../Core/PokeParty.h"
#include "Core/Singleton.h"

class PBattleManager;

enum EBattleType
{
	BT_Wild,
	BT_Trainer,
	BT_Gym
};

struct SBattleEvent
{
	EBattleType	   Type;
	uint32_t	   Id;
	PPokemonParty* Party;
};

class PBattleManager : public ISingleton<PBattleManager>
{
	EBattleType mType = BT_Wild;

	SPokemon* mPlayerPokemon = nullptr;
	SPokemon* mBattlePokemon = nullptr;

	PPokemonParty* mBattleParty = nullptr;

public:
	void StartBattle(const SBattleEvent& Event)
	{
		mPlayerPokemon = GetPlayerParty()->Get(0);

		mType = Event.Type;
		if (mType == BT_Wild)
		{
			mBattlePokemon = new SPokemon(PPokedexManager::Instance()->Get(Event.Id), 1, 0);
		}
		else
		{
			mBattlePokemon = Event.Party->Get(0);
		}
	}

	void EndBattle()
	{
		mPlayerPokemon = nullptr;
		if (mType == BT_Wild)
		{
			std::free(mBattlePokemon);
		}
		mBattlePokemon = nullptr;
		mBattleParty = nullptr;
	}

	EBattleType GetBattleType() { return mType; }
	void		SetBattleType(EBattleType NewType) { mType = NewType; }

	SPokemon* GetPlayerPokemon() const
	{
		return mPlayerPokemon;
	}

	void SetPlayerPokemon(SPokemon* PlayerPokemon)
	{
		mPlayerPokemon = PlayerPokemon;
	}

	SPokemon* GetBattlePokemon() const
	{
		return mBattlePokemon;
	}

	void SetBattlePokemon(SPokemon* BattlePokemon)
	{
		mBattlePokemon = BattlePokemon;
	}

	PPokemonParty* GetBattleParty() const
	{
		return mBattleParty;
	}

	void SetBattleParty(PPokemonParty* BattleParty)
	{
		mBattleParty = BattleParty;
	}
};
