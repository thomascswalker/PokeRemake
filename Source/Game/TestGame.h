#pragma once

#include "../Runtime/Engine/Game.h"

class TestGame : public PGame
{
public:
	TestGame();
	~TestGame() override {}
	void Tick(float DeltaTime) override {}
};
