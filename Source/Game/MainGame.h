#pragma once

#include "Core/PokeParty.h"
#include "Core/PokeStorage.h"
#include "Engine/Game.h"
#include "Interface/GameHUD.h"

class PMainGame : public PGame
{
	// Properties stored/accessible across game modes
	PGameHUD*		mHUD = nullptr;
	PPokemonParty	mPlayerParty;
	PPokemonStorage mPlayerStorage;

public:
	PMainGame();
	~PMainGame() override {}

	bool PreStart() override;
	bool Start() override;
	void OnKeyUp(SInputEvent* Event) override;

	PGameHUD* GetHUD() { return mHUD; }
};
