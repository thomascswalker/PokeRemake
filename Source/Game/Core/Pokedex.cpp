#include "Pokedex.h"

#include "Core/Files.h"
#include "Core/Json.h"

void PPokedexManager::Init()
{
	const auto	FileName = Files::FindFile("Pokedex.JSON");
	std::string Buffer;
	if (!Files::ReadFile(FileName, Buffer))
	{
		LogError("Failed to read Pokedex file");
		return;
	}
	JSON Data = JSON::parse(Buffer);

	for (const auto& Item : Data)
	{
		auto NewDef = SPokemonDef(Item);
		Defs.Add(NewDef);
	}
}

SPokemonDef* PPokedexManager::Get(int32_t Index) { return &Defs[Index - 1]; }

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
