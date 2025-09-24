#pragma once

#include "Engine/Game.h"
#include "Interface/DialogBox.h"

class PMainGame : public PGame
{
	PDialogBox* mDialogBox;

public:
	~PMainGame() override {}

	bool PreStart() override;

	bool HandleGameEvent(SGameEvent& Event) override;

	void ShowDialogBox(const std::string& Text);
	void CloseDialogBox();
};
