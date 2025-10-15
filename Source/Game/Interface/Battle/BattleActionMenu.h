#pragma once
#include "../../Battle/BattleManager.h"
#include "../Frame.h"
#include "Interface/Widget.h"

class PBattleActionMenu : public PFrame
{
public:
	PBattleActionMenu();
	~PBattleActionMenu() override;

	void Draw(const PRenderer* Renderer) const override;
};