#pragma once

#include "Core/PokeParty.h"
#include "Core/PokeStorage.h"
#include "Engine/Game.h"
#include "Interface/GameHUD.h"

class PMainGame : public PGame
{
	PGameHUD* mHUD = nullptr;

	PPokemonParty	mPlayerParty;
	PPokemonStorage mPlayerStorage;

public:
	PMainGame();
	~PMainGame() override {}

	bool PreStart() override;
	bool Start() override;

	PGameHUD* GetHUD() { return mHUD; }

	bool HandleGameEvent(SGameEvent& Event) override;

	bool StartDialogBox(SGameEvent* Event);
	bool EndDialogBox();

	bool StartBattle(SGameEvent* Event);
	bool EndBattle();

	void OnKeyUp(SInputEvent* Event) override;
};
