#pragma once

#include "Engine/Game.h"

class TestGame : public PGame
{
public:
	~TestGame() override {}

	bool PreStart() override;
};
