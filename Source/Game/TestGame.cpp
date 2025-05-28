#include "TestGame.h"

#include "Engine/Actors/Character.h"

TestGame::TestGame()
{
	mWorld->ConstructActor<PGrid>();
	mWorld->ConstructActor<PCharacter>();
}