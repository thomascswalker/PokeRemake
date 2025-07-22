#include "TestGame.h"

#include "Core/Files.h"
#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/Map.h"
#include "Engine/Serializer.h"
#include "Engine/Texture.h"

bool TestGame::PreStart()
{
	LogDebug("PreStart: Loading textures.");
	PTextureManager::LoadAllTextures();

	LoadAllTilesets();

	// Load the map from JSON
	PMapManager::LoadMap(MAP_PALLET_TOWN);

	LogDebug("PreStart: Constructing actors.");
	auto Player = ConstructActor<PPlayerCharacter>();
	GetWorld()->SetPlayerCharacter(Player);
	Player->MoveToTile(8, 8);

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