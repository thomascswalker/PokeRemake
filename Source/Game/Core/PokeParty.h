#pragma once

#include "Pokedex.h"
#include "Pokemon.h"

enum EPartyType
{
	PT_Player,
	PT_Trainer,
	PT_Wild
};

class PPokemonParty
{
	std::array<SPokemon*, 6> mPokemon;
	EPartyType				 mType;

	int32_t GetFirstAvailableSlot() const
	{
		for (int32_t Index = 0; Index < 6; Index++)
		{
			if (!mPokemon[Index])
			{
				return Index;
			}
		}
		return -1;
	}

public:
	PPokemonParty(EPartyType Type) : mType(Type)
	{
		mPokemon.fill(nullptr);
	}

	PPokemonParty(const PPokemonParty& Party)
	{
		mPokemon.fill(nullptr);
		for (int32_t Index = 0; Index < 6; Index++)
		{
			mPokemon[Index] = Party.mPokemon[Index];
		}
		mType = Party.mType;
	}

	EPartyType GetType() const { return mType; }
	void	   SetType(EPartyType Type) { mType = Type; }

	SPokemon* Get(uint32_t Index)
	{
		return mPokemon[Index];
	}

	bool Add(SPokemon* Pokemon)
	{
		int32_t Slot = GetFirstAvailableSlot();
		if (Slot == -1)
		{
			return false;
		}
		mPokemon[Slot] = Pokemon;
		return true;
	}

	bool Insert(SPokemon* Pokemon, uint32_t Index)
	{
		if (mPokemon[Index])
		{
			return false;
		}
		mPokemon[Index] = Pokemon;
		return true;
	}

	void Swap(uint32_t Index1, uint32_t Index2)
	{
		std::swap(mPokemon[Index1], mPokemon[Index2]);
	}
};

PPokemonParty* GetPlayerParty();