#pragma once

#include "Engine/Texture.h"

#include "Pokedex.h"

class SPokemon
{
	SPokemonDef mDef;

	uint32_t mHp;
	uint32_t mLevel;
	uint32_t mExperience;

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
};
