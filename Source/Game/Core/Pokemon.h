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
	SMoveDef* GetDef() { return &mDef; }
	uint32_t  GetPP() { return mPP; }
	uint32_t  DecrementPP()
	{
		if (mPP == 0)
		{
			return 0;
		}
		return mPP--;
	}
	void SetDisabled(bool State) { mDisabled = State; }
	bool GetDisabled() { return mDisabled; }
};

class SPokemon : public ISerializable
{
	SPokemonDef mDef;

	uint32_t mHp;
	uint32_t mLevel;
	uint32_t mExperience;

	std::array<std::shared_ptr<SPokemonMove>, 4> mMoves;

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

	bool AddMove(const SMoveDef* Def)
	{
		if (!Def)
		{
			LogError("Move definition is invalid.");
			return false;
		}
		auto NewIndex = GetMoveCount() - 1;
		if (NewIndex >= 3)
		{
			LogError("Unable to learn more than 4 moves.");
			return false;
		}
		// auto NewMove = std::make_shared<SPokemonMove>(Def);
		// mMoves[NewIndex] = NewMove;
		return true;
	}

	SPokemonMove* GetMove(int32_t Index)
	{
		if (Index < 0 || Index >= 4)
		{
			LogError("Move index {} out of bounds.", Index);
			return nullptr;
		}
		return mMoves[Index].get();
	}

	int32_t GetMoveCount()
	{
		int32_t Count = 0;
		for (auto& Move : mMoves)
		{
			if (Move)
			{
				Count++;
			}
		}
		return Count;
	}

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
			if (Move.get() == nullptr)
			{
				continue;
			}
			Result["Moves"].push_back(Move->Serialize());
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
			for (auto [IterIndex, MoveData] : std::views::enumerate(Json["Moves"]))
			{
				mMoves[IterIndex]->Deserialize(MoveData);
			}
		}
	}
};
