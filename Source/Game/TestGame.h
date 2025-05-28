#pragma once

#include "../Runtime/Engine/Game.h"

class TestGame : public PGame
{
public:
	TestGame() = default;
	~TestGame() override {}
	void PreStart() override;
};
