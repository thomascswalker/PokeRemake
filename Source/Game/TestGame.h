#pragma once

#include "Engine/Game.h"

class TestGame : public PGame
{
public:
	TestGame() = default;
	~TestGame() override {}
	void PreStart() override;
};
