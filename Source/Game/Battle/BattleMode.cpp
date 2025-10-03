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
					HandleChangeActionSelection(Event->KeyUp - SDLK_RIGHT);
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

	uint8_t CurrentPosition = static_cast<uint8_t>(GBattleManager->GetAction());

	bool X = (bool)(CurrentPosition % 2);
	bool Y = (bool)(CurrentPosition / 2);

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
		case 1: // LEFT
			X = !X;
			break;
		case 2: // DOWN
		case 3: // UP
			Y = !Y;
			break;
		default:
			break;
	}

	uint8_t NewX = (uint8_t)X;
	uint8_t NewY = (uint8_t)Y;

	uint8_t NewPosition = (NewY * 2) + NewX;

	GBattleManager->SetAction(static_cast<EBattleAction>(NewPosition));
}

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
