#include "Pokedex.h"

#include "Core/Files.h"
#include "Core/Json.h"

bool PPokedexManager::LoadDefs()
{
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
		Defs.Add(NewDef);
	}
	return true;
}

bool PPokedexManager::LoadSprites()
{
	return true;
}

bool PPokedexManager::Init()
{
	if (!LoadDefs())
	{
		LogError("Failed to load Pokemon definitions.");
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
SPokemonDef* PPokedexManager::Get(int32_t Index)
{
	return &Defs[Index];
}

SPokemonDef* PPokedexManager::GetById(int32_t Id) { return &Defs[Id - 1]; }

SPokemonDef* PPokedexManager::GetByName(const std::string& Name)
{
	for (auto& Def : Defs)
	{
		if (Def.Name == Name)
		{
			return &Def;
		}
	}
	return nullptr;
}
