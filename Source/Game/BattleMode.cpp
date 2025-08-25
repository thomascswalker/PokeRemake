#include "BattleMode.h"

#include "BattleView.h"

#include "Application/Application.h"

#include "Engine/Game.h"

bool PBattleMode::Load()
{
    GetApplication()->SetInputContext(Game::Context::Battle);

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
            GetGame()->SetCurrentGameMode("MapMode");
            break;
        }
    default: break;
    }
}
