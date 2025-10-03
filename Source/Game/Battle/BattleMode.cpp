#include "BattleMode.h"

#include "Application/Application.h"
#include "Core/GameConstants.h"
#include "Engine/Game.h"
#include "Modes/MapMode.h"

#include "MainGame.h"

PBattleMode::PBattleMode()
{
	mState.Data = SBattleContext::Schema();
	GBattleManager = &mBattleManager;
}

bool PBattleMode::PreStart()
{
	mBattleManager.PreStart();
	return true;
}

bool PBattleMode::Start()
{
	mBattleManager.Start();
	return true;
}

bool PBattleMode::Load()
{
	SetInputContext(IC_Battle);

	auto Id = mState.Get<int32_t>(STATE_BATTLE_ID);
	mBattleManager.StartTrainerBattle(Id);
	// mBattleManager.StartWildBattle(ID_MAGMAR, 50);
	mBattleManager.SetPlayerMon(GPlayerParty->Get(0));

	// Start with Select Action state
	GBattleManager->SetState(EBattleState::SelectAction);

	// Start the main battle HUD
	mHUD = GEngine->GetGameAs<PMainGame>()->GetHUD();
	mHUD->StartBattleHUD();

	return true;
}

bool PBattleMode::Unload()
{
	return true;
}

void PBattleMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			{
				mHUD->EndBattleHUD();

				if (!HandleGameEvent({ this, EGameEventType::BattleEnd }))
				{
					Event->Invalidate();
				}

				break;
			}
		case SDLK_RIGHT:
		case SDLK_LEFT:
		case SDLK_DOWN:
		case SDLK_UP:
			switch (GBattleManager->GetState())
			{
				case EBattleState::SelectAction:
					HandleChangeActionSelection(Event->KeyDown - SDLK_RIGHT);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

bool PBattleMode::HandleGameEvent(const SGameEvent& GameEvent)
{
	switch (GameEvent.Type)
	{
		case EGameEventType::BattleEnd:
			{
				if (!GEngine->GetGame()->SetAndLoadCurrentGameMode(MAP_MODE))
				{
					return false;
				}
				break;
			}
		default:
			break;
	}
	return false;
}

void PBattleMode::HandleChangeActionSelection(uint8_t Direction)
{
	// 0 RIGHT
	// 1 LEFT
	// 2 DOWN
	// 3 UP

	uint8_t NewDirection = static_cast<uint8_t>(GBattleManager->GetAction());

	/*
	 * ------------------------
	 * |    0     |     1     |
	 * -----------|-----------|
	 * |    2     |     3     |
	 * ------------------------
	 */
	switch (Direction)
	{
		case 0: // RIGHT
			NewDirection += 1;
			NewDirection %= 2;
			break;
		case 1: // LEFT
			NewDirection -= 1;
			NewDirection = std::abs(NewDirection % 2);
			break;
		case 2: // DOWN
			NewDirection += 2;
			NewDirection %= 4;
			break;
		case 3: // UP
			NewDirection -= 2;
			NewDirection = std::abs(NewDirection % 4);
			break;
		default:
			break;
	}

	GBattleManager->SetAction(static_cast<EBattleAction>(NewDirection));
}

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
