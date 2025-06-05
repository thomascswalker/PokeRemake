#include "TestGame.h"

#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/Texture.h"

void TestGame::PreStart()
{
	PGame::PreStart();

	LogDebug("PreStart: Constructing actors.");
	if (const auto W = GetWorld())
	{
		W->ConstructActor<PCharacter>()->MoveToTile(10, 10);
		W->ConstructActor<PPlayerCharacter>()->MoveToTile(12, 10);
	}

	LogDebug("PreStart: Loading textures.");
	PTextureManager::Load("Ash.png");
	PTextureManager::Load("Gary.png");
	PTextureManager::Load("PalletTown.png");
}

void TestGame::Start()
{
	PGame::Start();
	LogDebug("TestGame started.");
}