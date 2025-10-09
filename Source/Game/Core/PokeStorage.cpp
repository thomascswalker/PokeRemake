#include "PokeStorage.h"

PPokemonStorage* GPlayerStorage = nullptr;

SPokemon* PPokemonStorage::Construct(uint32_t Id, uint32_t Level, uint32_t Experience)
{
	auto Dex = PPokedexManager::Instance();
	auto Def = Dex->GetMonById(Id);
	auto NewMon = Add({ *Def, Level, Experience });
	for (auto MoveId : Def->DefaultMoves)
	{
		auto Move = Dex->GetMoveById(MoveId);
		if (Move != nullptr)
		{
			NewMon->AddMove(Move);
		}
	}
	return &mPokemon.back();
}

SPokemon* PPokemonStorage::Add(const SPokemon& Pokemon)
{
	mPokemon.push_back(Pokemon);
	return &mPokemon.back();
}

SPokemon* PPokemonStorage::Get(int32_t Index)
{
	return &mPokemon[Index];
}

JSON PPokemonStorage::Serialize() const
{
	JSON Result = JSON::array();

	for (const auto& Pokemon : mPokemon)
	{
		Result.push_back(Pokemon.Serialize());
	}

	return Result;
}

void PPokemonStorage::Deserialize(const JSON& Json)
{
	for (const auto& Pokemon : Json)
	{
		SPokemon* Mon = &mPokemon.emplace_back();
		Mon->Deserialize(Pokemon);
	}
}
