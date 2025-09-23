#pragma once

#include "Pokedex.h"

struct SPokemon
{
	SPokeDef Def;
	uint32_t Level;
	uint32_t Experience;
};

class PPokeStorage
{
	std::vector<SPokemon> mPokemon;

public:
	bool Add(const SPokemon& Pokemon)
	{
		mPokemon.push_back(Pokemon);
		return true;
	}

	SPokemon* Get(int32_t Index)
	{
		return &mPokemon[Index];
	}
};

class PParty
{
	std::array<SPokemon*, 6> mPokemon;

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
	PParty()
	{
		mPokemon.fill(nullptr);
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

	SPokemon* Get(uint32_t Index)
	{
		return mPokemon[Index];
	}
};