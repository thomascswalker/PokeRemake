#pragma once

#include "Engine/Actors/Character.h"
#include "Engine/Game.h"

class TestGame : public PGame
{
public:
	TestGame() = default;
	~TestGame() override {}
	bool PreStart() override;
	void Start() override;
};
