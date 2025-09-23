#include "BattleMode.h"

#include "Application/Application.h"
#include "Engine/Game.h"

#include "BattleView.h"

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
				GetGame()->SetAndLoadCurrentGameMode("MapMode");
				break;
			}
		default:
			break;
	}
}
