#include "TestGame.h"

#include "Core/Files.h"
#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/Serializer.h"
#include "Engine/Texture.h"

bool TestGame::PreStart()
{
	LogDebug("PreStart: Loading textures.");
	PTextureManager::LoadAllTextures();

	LoadAllTilesets();

	// Load the map from JSON
	LogDebug("PreStart: Loading map.");
	std::string Data;
	auto		FileName = Files::FindFile("PalletTown.JSON");
	if (!Files::ReadFile(FileName, Data))
	{
		return false;
	}
	json		JsonData = json::parse(Data.data());
	PSerializer Serializer;
	Serializer.Deserialize(JsonData);

	LogDebug("PreStart: Constructing actors.");
	if (const auto W = GetWorld())
	{
		W->ConstructActor<PCharacter>()->MoveToTile(10, 6);
		W->ConstructActor<PPlayerCharacter>()->MoveToTile(8, 8);
	}

	return true;
}

void TestGame::Start()
{
	PGame::Start();
	LogDebug("TestGame started.");

	for (const auto& Name : PTextureManager::GetTextures() | std::views::keys)
	{
		LogDebug("Texture: {}", Name.c_str());
	}
}