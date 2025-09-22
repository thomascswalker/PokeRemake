#include "MapMode.h"

#include "Application/Application.h"
#include "Interface/Button.h"

#define PLAYER_MAP		"MapName"
#define PLAYER_POSITION "PlayerPosition"

PMapMode::PMapMode()
{
	// Convenience vars
	mWorld = GetWorld();
	mMapManager = GetMapManager();

	mSaveState[PLAYER_MAP] = MAP_PALLET_TOWN;
	mSaveState[PLAYER_POSITION] = JSON::array({ 800, 800 });

	mMapManager->GameMapLoaded.AddRaw(this, &PMapMode::OnGameMapLoaded);
	mMapManager->GameMapUnloaded.AddRaw(this, &PMapMode::OnGameMapUnloaded);
}

bool PMapMode::Load()
{
	LogDebug("Load Save State: {}", mSaveState.dump(2).c_str());
	// Load the map from JSON
	auto Map = mMapManager->LoadMap(mSaveState[PLAYER_MAP], false);

	LogDebug("PreStart: Constructing actors.");
	auto Player = ConstructActor<PPlayerCharacter>();
	GetWorld()->SetPlayerCharacter(Player);

	auto	 JsonPosition = mSaveState[PLAYER_POSITION];
	FVector2 Position(JsonPosition);
	Player->GetMovementComponent()->SnapToPosition(Position, Map);

	SetInputContext(IC_Default);
	return true;
}

bool PMapMode::Unload()
{
	auto Player = mWorld->GetPlayerCharacter();

	// Current position
	auto PlayerPosition = Player->GetPosition2D();
	mSaveState[PLAYER_POSITION] = PlayerPosition.ToJson();

	// Current map
	auto Map = mMapManager->GetMapAtPosition(PlayerPosition);
	if (!Map)
	{
		LogError("No map at position: {}", PlayerPosition.ToString().c_str());
		return false;
	}
	mSaveState[PLAYER_MAP] = Map->GetMapName();

	// Destroy all actors
	mWorld->DestroyAllActors();

	// Clear all maps from the world
	mMapManager->ClearMaps();

	// Unset the player character
	mWorld->SetPlayerCharacter(nullptr);

	LogDebug("Unload Save State: {}", mSaveState.dump(2).c_str());

	return true;
}

void PMapMode::OnGameMapLoaded()
{
	LogInfo("Map loaded.");
}

void PMapMode::OnGameMapUnloaded()
{
	LogInfo("Map unloaded.");
}

void PMapMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			{
				if (!GetGame()->SetAndLoadCurrentGameMode("BattleMode"))
				{
					Event->Invalidate();
				}
				break;
			}
		default:
			break;
	}
}
