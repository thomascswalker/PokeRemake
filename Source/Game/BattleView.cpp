#include "BattleView.h"

#include "Engine/World.h"

void PBattleView::Start()
{
    if (const auto W = GetWorld())
    {
        mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
    }
}
