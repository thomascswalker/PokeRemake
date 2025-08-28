#include "TestGame.h"

#include "BattleMode.h"
#include "MapMode.h"

#include "Application/Application.h"

TestGame::TestGame()
{
	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();
}
