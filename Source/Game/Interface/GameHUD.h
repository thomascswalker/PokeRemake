#pragma once

#include "Engine/Dialog.h"
#include "Interface/HUD.h"
#include "Interface/Spacer.h"

#include "BattleHUD.h"
#include "DialogBox.h"

/**
 * @brief The main interface widget which holds all other interface widgets as children. This is
 * the case across all game modes.
 */
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

	void		StartBattleHUD();
	void		EndBattleHUD();
	PBattleHUD* GetBattleHUD() const;
};
