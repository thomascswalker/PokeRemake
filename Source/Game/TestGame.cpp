#include "TestGame.h"

#include "Application/Application.h"

#include "BattleMode.h"
#include "MapMode.h"

bool TestGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}
	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();
	return true;
}
