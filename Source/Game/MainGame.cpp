#include "MainGame.h"

#include "Application/Application.h"
#include "Core/Pokedex.h"
#include "Core/PokeParty.h"
#include "Core/PokeStorage.h"
#include "Modes/BattleMode.h"
#include "Modes/MapMode.h"

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
	// Load the pokedex
	auto Mgr = PPokedexManager::Instance();
	Mgr->Init();

	auto PlayerStorage = GetPlayerStorage();
	auto Mon = PlayerStorage->Construct(ID_BULBASAUR);

	auto PlayerParty = GetPlayerParty();
	PlayerParty->Add(Mon);

	return PGame::Start();
}
