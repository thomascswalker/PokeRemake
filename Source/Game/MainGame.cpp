#include "MainGame.h"

#include "Application/Application.h"

#include "BattleMode.h"
#include "MapMode.h"

bool PMainGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}
	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();
	return true;
}
