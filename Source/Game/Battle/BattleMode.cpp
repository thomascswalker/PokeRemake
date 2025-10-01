#include "BattleMode.h"

#include "Application/Application.h"
#include "Core/GameConstants.h"
#include "Engine/Game.h"
#include "Modes/MapMode.h"

#include "MainGame.h"

PBattleMode::PBattleMode()
{
	mState = SBattleContext::Schema();
}
bool PBattleMode::Start()
{
	mBattleManager.Start();
	return true;
}

bool PBattleMode::Load()
{
	SetInputContext(IC_Battle);
	GBattleManager = &mBattleManager;

	auto Id = mState["BattleId"].get<int32_t>();
	mBattleManager.SetBattleId(Id);
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
