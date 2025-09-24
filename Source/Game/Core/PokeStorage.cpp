#include "PokeStorage.h"

static PPokemonStorage gPlayerStorage;

SPokemon* PPokemonStorage::Construct(uint32_t Id, uint32_t Level, uint32_t Experience)
{
	auto Def = PPokedexManager::Instance()->Get(Id);
	mPokemon.push_back({ *Def, Level, Experience });
	return &mPokemon.back();
}

bool PPokemonStorage::Add(const SPokemon& Pokemon)
{
	mPokemon.push_back(Pokemon);
	return true;
}

SPokemon* PPokemonStorage::Get(int32_t Index)
{
	return &mPokemon[Index];
}

PPokemonStorage* GetPlayerStorage()
{
	return &gPlayerStorage;
}