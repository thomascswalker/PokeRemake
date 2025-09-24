#include "MapMode.h"

#include "Application/Application.h"
#include "Battle/BattleManager.h"
#include "Interface/Button.h"

#include "BattleMode.h"

#define PLAYER_MAP		"MapName"
#define PLAYER_POSITION "PlayerPosition"

PMapMode::PMapMode()
{
	// Convenience vars
	mWorld = GetWorld();
	mMapManager = GetMapManager();

	mSaveState[PLAYER_MAP] = MAP_PALLET_TOWN;
	mSaveState[PLAYER_POSITION] = JSON::array({ 800, 800 });

	mMapManager->GameMapStateChanged.AddRaw(this, &PMapMode::OnGameMapStateChanged);
	mGameHUD = ConstructWidget<PGameHUD>();
}

bool PMapMode::Load()
{
	auto HUD = GetHUD();
	if (!HUD)
	{
		LogError("HUD is invalid.");
		return false;
	}
	HUD->RemoveAllChildren();
	HUD->AddChild(mGameHUD);

	// Load the map from JSON
	auto Map = mMapManager->LoadMap(mSaveState[PLAYER_MAP], false);

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

	return true;
}

void PMapMode::OnGameMapStateChanged(EMapState State)
{
	switch (State)
	{
		case MS_Loading:
			break;
		case MS_Loaded:
			break;
		case MS_Unloading:
			TransitionOverlay = mWorld->ConstructWidget<PTransitionOverlay>();
			GetHUD()->AddChild(TransitionOverlay);
			TransitionOverlay->Fade(FM_Out);
			TransitionOverlay->FadedOut.AddRaw(this, &PMapMode::OnFadeOutComplete);
			TransitionOverlay->FadedIn.AddRaw(this, &PMapMode::OnFadeInComplete);
			break;
		case MS_Unloaded:
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
				if (!TryStartBattle())
				{
					Event->Invalidate();
				}
			}
		default:
			break;
	}
}
bool PMapMode::TryStartBattle()
{
	auto Game = GetGame();
	if (!Game->SetCurrentGameMode("BattleMode"))
	{
		return false;
	}
	Game->LoadCurrentGameMode();
	return StartBattle();
}

bool PMapMode::StartBattle()
{
	SBattleEvent Event;
	Event.Type = BT_Wild;
	Event.Id = ID_CHARMANDER;

	auto BattleMode = static_cast<PBattleMode*>(GetGame()->GetCurrentGameMode());
	if (!BattleMode)
	{
		LogError("BattleMode is not setup properly.");
		return false;
	}
	BattleMode->StartBattle(Event);
	return true;
}
