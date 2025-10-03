#pragma once
#include "../Battle/BattleManager.h"
#include "Interface/Widget.h"

class PBattleActionBox : public PWidget
{
	PTexture* mTexture = nullptr;

public:
	PBattleActionBox();

	void Draw(const PRenderer* Renderer) const override;
	void HandleBattleActionChange(EBattleAction Action);
};