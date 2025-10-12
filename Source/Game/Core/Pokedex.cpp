#include "Pokedex.h"

#include "Core/Files.h"
#include "Core/Json.h"

bool PPokedexManager::LoadPokemonDefs()
{
	LogDebug("Loading Pokemon definitions...");
	const auto	FileName = Files::FindFile("Pokedex.json");
	std::string Buffer;
	if (!Files::ReadFile(FileName, Buffer))
	{
		LogError("Failed to read 'Pokedex.json'");
		return false;
	}
	JSON Data = JSON::parse(Buffer);

	for (const auto& Item : Data)
	{
		auto NewDef = SPokemonDef(Item);
		Mons.Add(NewDef);
	}
	return true;
}

bool PPokedexManager::LoadMoveDefs()
{
	LogDebug("Loading Move definitions...");
	const auto	FileName = Files::FindFile("Moves.json");
	std::string Buffer;
	if (!Files::ReadFile(FileName, Buffer))
	{
		LogError("Failed to read 'Moves.json'");
		return false;
	}
	JSON Data = JSON::parse(Buffer);

	for (const auto& Item : Data)
	{
		auto NewDef = SMoveDef(Item);
		Moves.Add(NewDef);
	}
	return true;
}

bool PPokedexManager::LoadSprites()
{
	return true;
}

bool PPokedexManager::Init()
{
	if (!LoadPokemonDefs())
	{
		LogError("Failed to load Pokemon definitions.");
		return false;
	}

	if (!LoadMoveDefs())
	{
		LogError("Failed to load move definitions.");
		return false;
	}

	if (!LoadSprites())
	{
		LogError("Failed to load Pokemon sprites.");
		return false;
	}
	LogInfo("Loaded Pokemon definitions.");
	return true;
}
SPokemonDef* PPokedexManager::GetMonByIndex(int32_t Index)
{
	return &Mons[Index];
}

SPokemonDef* PPokedexManager::GetMonById(int32_t Id)
{
	return &Mons[Id - 1];
}

SPokemonDef* PPokedexManager::GetMonByName(const std::string& Name)
{
	for (auto& Def : Mons)
	{
		if (Def.Name == Name)
		{
			return &Def;
		}
	}
	return nullptr;
}

SMoveDef* PPokedexManager::GetMoveByIndex(int32_t Index)
{
	return &Moves[Index];
}

SMoveDef* PPokedexManager::GetMoveById(int32_t Id)
{
	return &Moves[Id - 1];
}
