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

				SMapContext Context;
				SGameEvent	GameEvent = { this, EGameEventType::BattleEnd, &Context };
				if (!HandleGameEvent(GameEvent))
				{
					Event->Invalidate();
				}

				break;
			}
		case SDLK_RIGHT:
			{
				GBattleManager->SwapNextBattleMon();
				mHUD->EndBattleHUD();
				mHUD->StartBattleHUD();
				break;
			}
		case SDLK_LEFT:
			{
				GBattleManager->SwapPrevBattleMon();
				mHUD->EndBattleHUD();
				mHUD->StartBattleHUD();
				break;
			}
		default:
			break;
	}
}

bool PBattleMode::HandleGameEvent(SGameEvent& GameEvent)
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

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
