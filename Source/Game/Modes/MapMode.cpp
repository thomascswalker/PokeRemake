#include "MapMode.h"

#include "Application/Application.h"
#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"
#include "Engine/Actors/Interactable.h"
#include "Interface/Button.h"

#include "MainGame.h"

static JSON GDefaultMapData = {
	{	  PLAYER_MAP,			  MAP_PALLET_TOWN },
	{ PLAYER_POSITION, JSON::array({ 800, 800 }) }
};

PMapMode::PMapMode()
{
	// Convenience vars
	mWorld = GWorld;
	mMapManager = GetMapManager();

	mMapManager->GameMapStateChanged.AddRaw(this, &PMapMode::OnGameMapStateChanged);

	mState = SMapContext::Schema();
}

std::string PMapMode::GetName()
{
	return MAP_MODE;
}

bool PMapMode::PreStart()
{
	mState = GDefaultMapData;
	return true;
}

bool PMapMode::Load()
{
	// Load the map from JSON
	auto Map = mMapManager->LoadMap(mState[PLAYER_MAP], false);

	auto Player = ConstructActor<PPlayerCharacter>();
	GWorld->SetPlayerCharacter(Player);

	auto	 JsonPosition = mState[PLAYER_POSITION];
	FVector2 Position(JsonPosition);
	Player->GetMovementComponent()->SnapToPosition(Position, Map);

	mHUD = GEngine->GetGameAs<PMainGame>()->GetHUD();

	SetInputContext(IC_Default);
	return true;
}

bool PMapMode::Unload()
{
	auto Player = mWorld->GetPlayerCharacter();

	// Current position
	auto PlayerPosition = Player->GetPosition2D();
	mState[PLAYER_POSITION] = PlayerPosition.ToJson();

	// Current map
	auto Map = mMapManager->GetMapAtPosition(PlayerPosition);
	if (!Map)
	{
		LogError("No map at position: {}", PlayerPosition.ToString().c_str());
		return false;
	}
	mState[PLAYER_MAP] = Map->GetMapName();

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
				SBattleContext Context;
				Context.Battle = GBattleManager->GetBattle(BATTLE_ID_GARY_OAK_LAB);

				SGameEvent GameEvent = { this, EGameEventType::BattleStart, &Context };
				if (!HandleGameEvent(GameEvent))
				{
					Event->Invalidate();
				}

				break;
			}
		default:
			break;
	}
}

bool PMapMode::HandleGameEvent(SGameEvent& GameEvent)
{
	JSON* GameState = nullptr;

	switch (GameEvent.Type)
	{
		case EGameEventType::Dialog:
			if (!mHUD->IsDialogBoxVisible())
			{
				mHUD->StartDialogBox(GameEvent.GetData<SInteractContext>()->Message);
			}
			else
			{
				mHUD->EndDialogBox();
			}
			break;
		case EGameEventType::BattleStart:
			// Update the battle state with the incoming battle ID to be loaded
			GameState = GEngine->GetGame()->GetGameMode(BATTLE_MODE)->GetState();
			GameState->operator[]("BattleId") = GameEvent.GetData<SBattleContext>()->Battle->Id;

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
