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
		W->ConstructActor<PCharacter>()->MoveToTile(2, 3);
		W->ConstructActor<PPlayerCharacter>();
	}

	LogDebug("PreStart: Loading textures.");
	PTextureManager::Load("Ash.png");
}

void TestGame::Start()
{
	PGame::Start();
	LogDebug("TestGame started.");
}