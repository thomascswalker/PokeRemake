#pragma once

#include "Engine/Serialization.h"
#include "Engine/Texture.h"

#include "Pokedex.h"

class SPokemonMove : public ISerializable
{
	SMoveDef mDef;
	uint32_t mPP = 0;
	bool	 mDisabled = false;

public:
	SPokemonMove(const SMoveDef* Def)
		: mDef(*Def), mPP(Def->PP)
	{
	}
};

class SPokemon : public ISerializable
{
	SPokemonDef mDef;

	uint32_t mHp;
	uint32_t mLevel;
	uint32_t mExperience;

	std::array<SPokemonMove, 4> mMoves;

public:
	SPokemon() : mHp{ 0 }, mLevel{ 0 }, mExperience(0) {}
	SPokemon(const SPokemonDef& Def, uint32_t Level, uint32_t Experience)
		: mDef(Def), mHp{ 0 }, mLevel(Level), mExperience(Experience) {}

	std::string GetDisplayName() const { return mDef.Name; }

	uint32_t	GetLevel() const { return mLevel; }
	void		SetLevel(uint32_t Level) { this->mLevel = Level; }
	void		LevelUp() { this->mLevel++; }
	std::string GetDisplayLevel() const { return std::format("L{}", this->mLevel); }

	uint32_t GetExperience() const { return mExperience; }
	void	 AddExperience(uint32_t Exp) { this->mExperience = Exp; }

	PTexture* GetFrontTexture() const
	{
		return mDef.Front;
	}

	PTexture* GetBackTexture() const
	{
		return mDef.Back;
	}

	JSON Serialize() const override
	{
		JSON Result;

		Result["Id"] = mDef.Id;
		Result["Level"] = mLevel;
		Result["Experience"] = mExperience;
		Result["Moves"] = JSON::array();
		for (auto Move : mMoves)
		{
			if (!Move)
			{
				Result["Moves"].push_back(-1);
			}
			else
			{
				Result["Moves"].push_back(Move->Id);
			}
		}

		return Result;
	}

	void Deserialize(const JSON& Json) override
	{
		auto Id = Json["Def"].get<int32_t>();
		mDef = *PPokedexManager::Instance()->GetMonById(Id);
		mLevel = Json["Level"];
		if (Json.contains("Experience"))
		{
			mExperience = Json["Experience"];
		}
		if (Json.contains("Moves"))
		{
			for (auto [IterIndex, MoveId] : std::views::enumerate(Json["Moves"]))
			{
				mMoves[IterIndex] = PPokedexManager::Instance()->GetMoveById(MoveId);
			}
		}
	}
};
