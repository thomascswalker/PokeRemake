#include "MainGame.h"

#include "Application/Application.h"

#include "BattleMode.h"
#include "MapMode.h"

PMainGame::PMainGame()
{
}

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
bool PMainGame::Start()
{
	SPokeDef Bulbasaur = *Pokedex.Get(0);
	Storage.Add({ Bulbasaur, 1, 0 });
	Party.Add(Storage.Get(0));
	return PGame::Start();
}
