#pragma once

#include "../Battle/BattleHUD.h"
#include "Interface/HUD.h"
#include "Interface/Spacer.h"

#include "DialogBox.h"

class PGameHUD : public PHUD
{
	PSpacer*	mSpacer = nullptr;
	PDialogBox* mDialogBox = nullptr;
	PBattleHUD* mBattleHUD = nullptr;

public:
	PGameHUD();
	~PGameHUD() override = default;

	void StartDialogBox(const std::string& Text);
	void EndDialogBox();
	bool IsDialogBoxVisible();

	void StartBattleHUD();
	void EndBattleHUD();
};
