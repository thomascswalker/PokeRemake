#include "TestGame.h"

#include "Core/Files.h"
#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/Serializer.h"
#include "Engine/Texture.h"

void TestGame::PreStart()
{
	LogDebug("PreStart: Loading textures.");
	PTextureManager::Load("Ash.png");
	PTextureManager::Load("Gary.png");
	PTextureManager::Load("PalletTown.png");
	PTextureManager::Load("PalletTownInterior.png");
	PTextureManager::Load("Route1.png");

	// Load the map from JSON
	LogDebug("PreStart: Loading map.");
	std::string Data;
	auto		FileName = Files::FindFile("Overworld.JSON");
	if (!Files::ReadFile(FileName, Data))
	{
		return;
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
}

void TestGame::Start()
{
	PGame::Start();
	LogDebug("TestGame started.");
}