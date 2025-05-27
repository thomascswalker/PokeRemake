#pragma once

#include "Core/Game.h"
#include "Engine/Actors/Character.h"

class TestGame : public PGame
{
public:
	TestGame()
	{
		mWorld->ConstructActor<PGrid>();
		mWorld->ConstructActor<PCharacter>();
	}
	~TestGame() override {}
	void Tick(float DeltaTime) override {}
};
