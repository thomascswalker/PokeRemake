#include "BattleView.h"

#include "Engine/World.h"

bool PBattleView::Start()
{
	if (const auto W = GetWorld())
	{
		mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
	}
	return true;
}
