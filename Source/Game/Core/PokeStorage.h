#pragma once

#include "Pokedex.h"
#include "Pokemon.h"

class PPokemonStorage
{
	std::vector<SPokemon> mPokemon;

public:
	SPokemon*			   Construct(uint32_t Id, uint32_t Level = 1, uint32_t Experience = 0);
	bool				   Add(const SPokemon& Pokemon);
	SPokemon*			   Get(int32_t Index);
	std::vector<SPokemon>& GetAll() { return mPokemon; }
};

extern PPokemonStorage* GPlayerStorage;