#include "BattleMode.h"

#include "Application/Application.h"
#include "Core/GameConstants.h"
#include "Engine/Game.h"
#include "Modes/MapMode.h"

#include "MainGame.h"

PBattleMode::PBattleMode()
{
	mState = SBattleContext::Schema();
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

	auto Id = mState["BattleId"].get<int32_t>();
	mBattleManager.SetCurrentBattleId(Id);
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
				GBattleManager->NextBattleMon();
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
				if (!GEngine->GetGame()->SetAndLoadCurrentGameMode("MapMode"))
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
