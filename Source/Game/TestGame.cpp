#include "TestGame.h"

#include "BattleMode.h"
#include "MapMode.h"

#include "Application/Application.h"
#include "Core/Logging.h"
#include "Engine/Actors/PlayerCharacter.h"
#include "Engine/MapManager.h"
#include "Engine/Texture.h"

TestGame::TestGame()
{
	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();
}

bool TestGame::PreStart()
{
	PGame::PreStart();

	LogDebug("PreStart: Loading textures.");
	PTextureManager::LoadAllTextures();
	LoadAllTilesets();
	if (!SetCurrentGameMode("MapMode"))
	{
		LogError("Failed to set current game mode.");
		return false;
	}
	return true;
}

