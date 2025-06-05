#include "TestGame.h"

#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/Texture.h"

void TestGame::PreStart()
{
	PGame::PreStart();

	if (const auto W = GetWorld())
	{
		W->ConstructActor<PCharacter>();
		W->ConstructActor<PPlayerCharacter>();
	}

	PTextureManager::Load("Ash.png");
}