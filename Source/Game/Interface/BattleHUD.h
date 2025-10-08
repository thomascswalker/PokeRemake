#pragma once

#include "Interface/Widget.h"

#include "BattleActionMenu.h"
#include "DialogBox.h"

#define NAME_FONT_SIZE 24

#define SPRITE_SIZE WINDOW_HEIGHT / 2

#define PLAYER_SPRITE_X 0
#define PLAYER_SPRITE_Y SPRITE_SIZE

#define BATTLE_SPRITE_X WINDOW_WIDTH - SPRITE_SIZE
#define BATTLE_SPRITE_Y 0

class PBattleHUD : public PWidget
{
	PDialogBox*		   mDialogBox = nullptr;
	PBattleActionMenu* mActionBox = nullptr;

public:
	PBattleHUD();
	~PBattleHUD() override;

	void Draw(const PRenderer* Renderer) const override;

	void ShowActionBox();
	void HideActionBox();

	void ShowDialogBox();
	void HideDialogBox();
};