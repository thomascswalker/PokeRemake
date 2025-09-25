#include "BattleMode.h"

#include "Application/Application.h"
#include "Engine/Game.h"

#include "BattleView.h"
#include "MainGame.h"

bool PBattleMode::Load()
{
	SetInputContext(IC_Battle);

	ConstructActor<PBattleView>();

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
				SGameEvent GameEvent = { this, EGameEventType::BattleEnd };
				GEngine->GetGame()->HandleGameEvent(GameEvent);

				if (!GEngine->GetGame()->SetAndLoadCurrentGameMode("MapMode"))
				{
					Event->Invalidate();
				}
				break;
			}
		default:
			break;
	}
}
