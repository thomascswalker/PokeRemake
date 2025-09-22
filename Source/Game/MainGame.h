#pragma once

#include "Engine/Game.h"

class PMainGame : public PGame
{
public:
	~PMainGame() override {}

	bool PreStart() override;
};
