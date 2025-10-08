#pragma once
#include "../Battle/BattleManager.h"
#include "Interface/Widget.h"

#include "Frame.h"

class PBattleActionMenu : public PFrame
{
	PTexture* mTexture = nullptr;

public:
	PBattleActionMenu();
	~PBattleActionMenu() override;

	void Draw(const PRenderer* Renderer) const override;
	void HandleBattleActionChange(EBattleAction Action);
};