#pragma once

#include "Core/Containers.h"
#include "Engine/Serialization.h"
#include "Engine/Texture.h"

#include "Pokedex.h"

class SPokemonMove : public ISerializable
{
	SMoveDef* mDef = nullptr;
	uint32_t  mPP = 0;
	bool	  mDisabled = false;

public:
	SPokemonMove() = default;
	~SPokemonMove() override = default;
	SMoveDef* GetDef() { return mDef; }
	void	  SetDef(SMoveDef* Def) { mDef = Def; }
	uint32_t  GetPP() { return mPP; }
	void	  SetPP(uint32_t PP) { mPP = PP; }
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

	JSON Serialize() const override
	{
		return JSON();
	}
	void Deserialize(const JSON& json) override
	{
	}
};

class SPokemon;

DECLARE_MULTICAST_DELEGATE(DOnFainted, SPokemon*);

class SPokemon : public ISerializable
{
	SPokemonDef mDef;

	uint32_t mHp;
	uint32_t mLevel;
	uint32_t mExperience;
	uint32_t mAttack;
	uint32_t mDefense;
	uint32_t mSpAttack;
	uint32_t mSpDefense;
	uint32_t mSpeed;

	std::array<std::shared_ptr<SPokemonMove>, 4> mMoves;

public:
	DOnFainted OnFainted;

	SPokemon() : mHp{ 0 }, mLevel{ 0 }, mExperience(0) {}
	SPokemon(const SPokemonDef& Def, uint32_t Level, uint32_t Experience)
		: mDef(Def), mHp{ Def.MaxHp }, mLevel(Level), mExperience(Experience)
	{
		mAttack = Def.Attack;
		mDefense = Def.Defense;
		mSpeed = Def.Speed;
		mSpDefense = Def.Defense;
		mSpAttack = Def.SpAttack;
	}

	std::string GetDisplayName() const { return mDef.Name; }
	std::string GetDisplayHp() const
	{
		return std::format("{}/{}", mHp, mDef.MaxHp);
	}

	uint32_t GetCurrentHp() const { return mHp; }
	uint32_t GetMaxHp() const { return mDef.MaxHp; }
	uint32_t Damage(uint32_t InDamage)
	{
		mHp = InDamage > mHp ? 0 : mHp - InDamage;
		return mHp;
	}
	uint32_t Heal(uint32_t InHeal)
	{
		mHp = std::min(mHp + InHeal, mDef.MaxHp);
		return mHp;
	}
	void FullHeal()
	{
		mHp = mDef.MaxHp;
	}
	bool IsUsable() const { return mHp > 0; }

	uint32_t GetAttack() const { return mAttack; }
	uint32_t GetDefense() const { return mDefense; }
	uint32_t GetSpeed() const { return mSpeed; }
	uint32_t GetSpDefense() const { return mSpDefense; }
	uint32_t GetSpAttack() const { return mSpAttack; }

	EPokeType GetType1() const { return mDef.Types[0]; }
	EPokeType GetType2() const { return mDef.Types[1]; }
	bool	  IsAnyType(EPokeType InType) const
	{
		return mDef.Types[0] == InType || mDef.Types[1] == InType;
	}

	uint32_t	GetLevel() const { return mLevel; }
	void		SetLevel(uint32_t Level) { this->mLevel = Level; }
	void		LevelUp() { this->mLevel++; }
	std::string GetDisplayLevel() const { return std::format("{}", this->mLevel); }

	uint32_t GetExperience() const { return mExperience; }
	void	 AddExperience(uint32_t Exp) { this->mExperience = Exp; }

	bool AddMove(SMoveDef* Def)
	{
		if (!Def)
		{
			LogError("Move definition is invalid.");
			return false;
		}
		auto NewIndex = GetNextFreeIndex();
		if (NewIndex > 3)
		{
			LogError("Unable to learn more than 4 moves.");
			return false;
		}
		auto NewMove = std::make_shared<SPokemonMove>();
		NewMove->SetDef(Def);
		NewMove->SetPP(Def->PP);
		mMoves[NewIndex] = NewMove;
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

	int32_t GetNextFreeIndex()
	{
		int32_t Index = 0;
		for (auto& Move : mMoves)
		{
			if (Move.get() == nullptr)
			{
				return Index;
			}
			Index++;
		}
		return 4;
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
