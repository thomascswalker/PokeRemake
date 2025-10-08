#include "MapMode.h"

#include "Application/Application.h"
#include "Battle/BattleManager.h"
#include "Battle/BattleMode.h"
#include "Core/GameConstants.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Actors/SceneryActor.h"
#include "Interface/Button.h"

#include "MainGame.h"

static JSON GDefaultMapData = {
	{	  PLAYER_MAP,			  MAP_PALLET_TOWN },
	{ PLAYER_POSITION, JSON::array({ 800, 800 }) }
};

PMapMode::PMapMode()
{
	GMapManager->GameMapStateChanged.AddRaw(this, &PMapMode::OnGameMapStateChanged);
	mState.Data = GDefaultMapData;
}

std::string PMapMode::GetName()
{
	return MAP_MODE;
}

bool PMapMode::PreStart()
{
	return true;
}

bool PMapMode::Load()
{
	// Load the map from JSON
	auto Map = GMapManager->LoadMap(mState.Get<std::string>(PLAYER_MAP), false);

	auto Player = ConstructActor<PPlayerCharacter>();
	Player->GetSpriteComponent()->GetSprite()->SetTexture(GTextureManager->Get(TEX_ASH));
	GWorld->SetPlayerCharacter(Player);

	auto NewPosition = FVector2(mState.GetRaw(PLAYER_POSITION));
	Player->GetMovementComponent()->SnapToPosition(NewPosition, Map, false);

	mHUD = GEngine->GetGameAs<PMainGame>()->GetHUD();

	SetInputContext(IC_Default);
	return true;
}

bool PMapMode::Unload()
{
	auto Player = GWorld->GetPlayerCharacter();

	// Current position
	auto PlayerPosition = Player->GetPosition2D();
	mState.Set(PLAYER_POSITION, PlayerPosition.ToJson());

	// Current map
	auto Map = GMapManager->GetMapAtPosition(PlayerPosition);
	if (!Map)
	{
		LogError("No map at position: {}", PlayerPosition.ToString().c_str());
		return false;
	}
	mState.Set(PLAYER_MAP, Map->GetMapName());

	// Destroy all actors
	GWorld->DestroyAllActors();

	// Clear all maps from the world
	GMapManager->ClearMaps();

	// Unset the player character
	GWorld->SetPlayerCharacter(nullptr);

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
			TransitionOverlay = GWorld->ConstructWidget<PTransitionOverlay>();
			GWorld->GetRootWidget()->AddChild(TransitionOverlay);
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
		TransitionOverlay->FadedOut.RemoveAll();
		GWorld->DestroyWidget(TransitionOverlay);
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

bool PMapMode::HandleGameEvent(const SGameEvent& GameEvent)
{
	switch (GameEvent.Type)
	{
		case EGameEventType::Dialog:
			if (!mHUD->IsDialogBoxVisible())
			{
				mHUD->StartDialogBox(GameEvent.GetData<SDialogContext>());
			}
			else
			{
				mHUD->EndDialogBox();
			}
			break;
		case EGameEventType::BattleStart:
			// Update the battle state with the incoming battle ID to be loaded
			GEngine->GetGameState(BATTLE_MODE)->Set(STATE_BATTLE_ID, GameEvent.GetData<SBattleContext>().Trainer->Id);

			// First load the new game mode
			if (!GEngine->GetGame()->SetAndLoadCurrentGameMode(BATTLE_MODE))
			{
				return false;
			}
			break;
		default:
			break;
	}
	return true;
}
