#pragma once

#include "../Battle/BattleHUD.h"
#include "Engine/Dialog.h"
#include "Interface/HUD.h"
#include "Interface/Spacer.h"

#include "DialogBox.h"

class PGameHUD : public PHUD
{
	PSpacer* mSpacer = nullptr;

	PDialogBox*	   mDialogBox = nullptr;
	SDialogContext mDialogContext;

	PBattleHUD* mBattleHUD = nullptr;

public:
	PGameHUD();
	~PGameHUD() override = default;

	void StartDialogBox(const SDialogContext& Context);
	void EndDialogBox();
	bool IsDialogBoxVisible();

	void StartBattleHUD();
	void EndBattleHUD();
};
