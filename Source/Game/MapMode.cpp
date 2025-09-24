#include "MapMode.h"

#include "Application/Application.h"
#include "Interface/Button.h"

#define PLAYER_MAP		"MapName"
#define PLAYER_POSITION "PlayerPosition"

PMapMode::PMapMode()
{
	// Convenience vars
	mWorld = GWorld;
	mMapManager = GetMapManager();

	mSaveState[PLAYER_MAP] = MAP_PALLET_TOWN;
	mSaveState[PLAYER_POSITION] = JSON::array({ 800, 800 });

	mMapManager->GameMapStateChanged.AddRaw(this, &PMapMode::OnGameMapStateChanged);
}

bool PMapMode::Load()
{
	LogDebug("Load Save State: {}", mSaveState.dump(2).c_str());
	// Load the map from JSON
	auto Map = mMapManager->LoadMap(mSaveState[PLAYER_MAP], false);

	LogDebug("PreStart: Constructing actors.");
	auto Player = ConstructActor<PPlayerCharacter>();
	GWorld->SetPlayerCharacter(Player);

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

void PMapMode::OnGameMapStateChanged(EMapState State)
{
	switch (State)
	{
		case MS_Loading:
			LogInfo("Map loading...");
			break;
		case MS_Loaded:
			LogInfo("Map loaded.");
			break;
		case MS_Unloading:
			LogInfo("Map unloading...");
			TransitionOverlay = mWorld->ConstructWidget<PTransitionOverlay>();
			LogWarning("Reimplement PGameHUD Add TransitionOverlay on PMapMode");
			// GetHUD<PGameHUD>()->AddChild(TransitionOverlay);
			TransitionOverlay->Fade(FM_Out);
			TransitionOverlay->FadedOut.AddRaw(this, &PMapMode::OnFadeOutComplete);
			TransitionOverlay->FadedIn.AddRaw(this, &PMapMode::OnFadeInComplete);
			break;
		case MS_Unloaded:
			LogInfo("Map unloaded.");
			break;
	}
}

void PMapMode::OnFadeInComplete()
{
	if (TransitionOverlay)
	{
		TransitionOverlay->Unparent();
		TransitionOverlay->FadedIn.RemoveAll();
		mWorld->DestroyWidget(TransitionOverlay);
		TransitionOverlay = nullptr;
	}
}

void PMapMode::OnFadeOutComplete()
{
	if (TransitionOverlay)
	{
		TransitionOverlay->Fade(FM_In);
	}
}

void PMapMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			{
				if (!GEngine->GetGame()->SetAndLoadCurrentGameMode("BattleMode"))
				{
					Event->Invalidate();
				}
				break;
			}
		default:
			break;
	}
}
