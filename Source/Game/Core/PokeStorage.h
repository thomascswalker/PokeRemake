#pragma once

#include "Engine/Serialization.h"

#include "Pokemon.h"

class PPokemonStorage : public ISerializable
{
	std::vector<SPokemon> mPokemon;

public:
	SPokemon*			   Construct(uint32_t Id, uint32_t Level = 1, uint32_t Experience = 0);
	SPokemon*			   Add(const SPokemon& Pokemon);
	SPokemon*			   Get(int32_t Index);
	std::vector<SPokemon>& GetAll() { return mPokemon; }
	void				   Clear() { mPokemon.clear(); }

	JSON Serialize() const override;
	void Deserialize(const JSON& Json) override;
};

extern PPokemonStorage* GPlayerStorage;