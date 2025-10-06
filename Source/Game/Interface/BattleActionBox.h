#pragma once
#include "../Battle/BattleManager.h"
#include "Interface/Widget.h"

class PBattleActionBox : public PWidget
{
	PTexture* mTexture = nullptr;

public:
	PBattleActionBox();
	~PBattleActionBox() override;

	void Draw(const PRenderer* Renderer) const override;
	void HandleBattleActionChange(EBattleAction Action);
};