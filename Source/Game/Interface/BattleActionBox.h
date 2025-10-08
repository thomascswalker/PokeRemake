#pragma once
#include "../Battle/BattleManager.h"
#include "Interface/Widget.h"

#include "Frame.h"

class PBattleActionBox : public PFrame
{
	PTexture* mTexture = nullptr;

public:
	PBattleActionBox();
	~PBattleActionBox() override;

	void Draw(const PRenderer* Renderer) const override;
	void HandleBattleActionChange(EBattleAction Action);
};