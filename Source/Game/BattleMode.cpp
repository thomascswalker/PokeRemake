#include "BattleMode.h"

#include "BattleView.h"

#include "Application/Application.h"

#include "Engine/Game.h"

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
    case SDLK_ESCAPE:
        {
            GetGame()->SetAndLoadCurrentGameMode("MapMode");
            break;
        }
    default: break;
    }
}
