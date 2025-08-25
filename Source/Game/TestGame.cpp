#include "TestGame.h"

#include "BattleMode.h"
#include "MapMode.h"

#include "Application/Application.h"
#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/MapManager.h"
#include "Engine/Texture.h"

TestGame::TestGame() : PGame()
{
	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();
}

bool TestGame::PreStart()
{
	LogDebug("PreStart: Loading textures.");
	PTextureManager::LoadAllTextures();
	LoadAllTilesets();
	return true;
}

