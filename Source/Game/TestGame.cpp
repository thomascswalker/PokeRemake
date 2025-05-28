#include "TestGame.h"

#include "Engine/Actors/Character.h"

void TestGame::PreStart()
{
	PGame::PreStart();

	if (const auto W = GetWorld())
	{
		W->ConstructActor<PGrid>();
		W->ConstructActor<PCharacter>();
	}
}