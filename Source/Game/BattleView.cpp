#include "BattleView.h"

#include "Engine/World.h"

bool PBattleView::Start()
{
	if (const auto W = GWorld)
	{
		mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
	}
	return true;
}
