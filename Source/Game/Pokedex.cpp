#include "Pokedex.h"

#include "Core/Files.h"
#include "Core/Json.h"

PPokedexManager::PPokedexManager()
{
	const auto	FileName = Files::FindFile("Pokedex.JSON");
	std::string Buffer;
	Files::ReadFile(FileName, Buffer);
	JSON Data = JSON::parse(Buffer);

	for (const auto& Item : Data)
	{
		auto NewDef = SPokeDef(Item);
		LogInfo("{}", NewDef.ToString().c_str());
		Defs.Add(NewDef);
	}
}