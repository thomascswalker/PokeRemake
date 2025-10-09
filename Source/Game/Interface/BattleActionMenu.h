#pragma once
#include "../Battle/BattleManager.h"
#include "Interface/Widget.h"

#include "Frame.h"

class PBattleActionMenu : public PFrame
{
public:
	PBattleActionMenu();
	~PBattleActionMenu() override;

	void Draw(const PRenderer* Renderer) const override;
};