#pragma once
#include "Frame.h"

class PBattlePPMenu : public PFrame
{
public:
	PBattlePPMenu();
	~PBattlePPMenu() override {};

	void Draw(const PRenderer* Renderer) const override;
};
