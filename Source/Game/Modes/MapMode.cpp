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

	mPlayerCharacter = ConstructActor<PPlayerCharacter>();
	mPlayerCharacter->GetSpriteComponent()->GetSprite()->SetTexture(GTextureManager->Get(TEX_ASH));
	GWorld->SetPlayerCharacter(mPlayerCharacter);

	auto NewPosition = FVector2(mState.GetRaw(PLAYER_POSITION));
	mPlayerCharacter->GetMovementComponent()->SnapToPosition(NewPosition, Map, false);

	mHUD = GEngine->GetGameAs<PMainGame>()->GetHUD();

	SetInputContext(IC_Default);
	return true;
}

bool PMapMode::Unload()
{
	// Current position
	auto PlayerPosition = mPlayerCharacter->GetPosition2D();
	mState.Set(PLAYER_POSITION, PlayerPosition.ToJson());
	mPlayerCharacter = nullptr;

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

void PMapMode::OnKeyDown(SInputEvent* Event)
{
	switch (Event->KeyDown)
	{
		case SDLK_Q:
			if (HandlePressB())
			{
				Event->Consume();
			}
			break;
		case SDLK_E:
			if (HandlePressA())
			{
				Event->Consume();
			}
			break;
		case SDLK_W:
		case SDLK_A:
		case SDLK_S:
		case SDLK_D:
		case SDLK_RIGHT:
		case SDLK_LEFT:
		case SDLK_DOWN:
		case SDLK_UP:
			if (HandlePressDPad(RemapKeyToDPad(Event->KeyDown)))
			{
				Event->Consume();
			}
			break;
		default:
			break;
	}
}
void PMapMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			if (HandleReleaseB())
			{
				Event->Consume();
			}
			break;
		case SDLK_E:
			if (HandleReleaseA())
			{
				Event->Consume();
			}
			break;
		case SDLK_W:
		case SDLK_A:
		case SDLK_S:
		case SDLK_D:
		case SDLK_RIGHT:
		case SDLK_LEFT:
		case SDLK_DOWN:
		case SDLK_UP:
			if (HandleReleaseDPad(RemapKeyToDPad(Event->KeyUp)))
			{
				Event->Consume();
			}
			break;
		default:
			break;
	}
}

bool PMapMode::HandlePressA()
{
	return PGameMode::HandlePressA();
}

bool PMapMode::HandleReleaseA()
{
	if (IsInputContext(IC_Default))
	{
		mPlayerCharacter->Interact();
		return true;
	}
	if (IsInputContext(IC_Dialog))
	{
		return HandleGameEvent(SGameEvent(this, EGameEventType::Dialog));
	}
	return false;
}

bool PMapMode::HandlePressB()
{
	return PGameMode::HandlePressB();
}

bool PMapMode::HandleReleaseB()
{
	return PGameMode::HandleReleaseB();
}

bool PMapMode::HandlePressDPad(EDPad Direction)
{
	return mPlayerCharacter->TryMove(Direction);
}

bool PMapMode::HandleReleaseDPad(EDPad Direction)
{
	return mPlayerCharacter->TryStop(Direction);
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
