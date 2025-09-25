#pragma once

#include "Engine/Game.h"
#include "Interface/GameHUD.h"

class PMainGame : public PGame
{
	PGameHUD* mHUD = nullptr;

public:
	~PMainGame() override {}

	bool PreStart() override;
	bool Start() override;

	bool HandleGameEvent(SGameEvent& Event) override;
};
