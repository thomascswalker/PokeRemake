#pragma once

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
	uint32_t GetLevel() const { return mLevel; }
	void	 SetLevel(uint32_t Level) { this->mLevel = Level; }
	void	 LevelUp() { this->mLevel++; }

	uint32_t GetExperience() const { return mExperience; }
	void	 AddExperience(uint32_t Exp) { this->mExperience = Exp; }
};
