#include "TestGame.h"

TestGame::TestGame()
{
	mWorld->ConstructActor<PGrid>();
	mWorld->ConstructActor<PCharacter>();
}