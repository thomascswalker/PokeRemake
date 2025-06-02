#include "TestGame.h"

#include "Engine/Actors/PlayerCharacter.h"

void TestGame::PreStart()
{
	PGame::PreStart();

	if (const auto W = GetWorld())
	{
		W->ConstructActor<PCharacter>();
		W->ConstructActor<PPlayerCharacter>();
	}
}