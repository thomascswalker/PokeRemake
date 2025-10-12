#pragma once

#include "Frame.h"

class PBattleMoveMenu : public PFrame
{
public:
	PBattleMoveMenu();

	void Draw(const PRenderer* Renderer) const override;
};
