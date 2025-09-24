#pragma once

#include "Interface/HUD.h"
#include "Interface/Spacer.h"

#include "DialogBox.h"

class PGameHUD : public PHUD
{
	PSpacer*	mSpacer = nullptr;
	PDialogBox* mDialogBox = nullptr;

public:
	PGameHUD();
	~PGameHUD() override = default;

	void DialogBox(const std::string& Text);
};
