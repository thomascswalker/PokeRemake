#pragma once

#include "Pokedex.h"
#include "Pokemon.h"

constexpr int32_t GMaxPartyCount = 6;
#define CHECK_PARTY_INDEX(Index) ASSERT(Index >= 0 && Index < GMaxPartyCount, "Index is out of range")

enum EPartyType
{
	PT_Player,
	PT_Trainer,
	PT_Wild
};

class PPokemonParty
{
	std::array<SPokemon*, 6> mPokemon;
	EPartyType				 mType = PT_Player;

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
	PPokemonParty()
	{
		mPokemon.fill(nullptr);
	};

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

	int32_t GetCount() const
	{
		int32_t Count = 0;
		for (int32_t Index = 0; Index < 6; Index++)
		{
			if (!mPokemon[Index])
			{
				Count++;
			}
		}
		return Count;
	}

	SPokemon* Get(uint32_t Index)
	{
		CHECK_PARTY_INDEX(Index);
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
		CHECK_PARTY_INDEX(Index);
		if (mPokemon[Index])
		{
			return false;
		}
		mPokemon[Index] = Pokemon;
		return true;
	}

	void Swap(uint32_t Index1, uint32_t Index2)
	{
		CHECK_PARTY_INDEX(Index1);
		CHECK_PARTY_INDEX(Index2);
		ASSERT(Index1 != Index2, "Index1 is not the same as the Index2");
		std::swap(mPokemon[Index1], mPokemon[Index2]);
	}

	void Clear() { mPokemon.fill(nullptr); }

	int32_t GetIndex(const SPokemon* Pokemon) const
	{
		for (int32_t Index = 0; Index < 6; Index++)
		{
			if (mPokemon[Index] == Pokemon)
			{
				return Index;
			}
		}
		return -1;
	}
};

extern PPokemonParty* GPlayerParty;