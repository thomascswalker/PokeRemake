#pragma once

#include "../Interface/DialogBox.h"
#include "Interface/Widget.h"

#include "BattleManager.h"

#define NAME_FONT_SIZE 20

#define SPRITE_SIZE WINDOW_DEFAULT_HEIGHT / 2

#define PLAYER_SPRITE_X 0
#define PLAYER_SPRITE_Y SPRITE_SIZE

#define BATTLE_SPRITE_X WINDOW_DEFAULT_WIDTH - SPRITE_SIZE
#define BATTLE_SPRITE_Y 0

class PBattleHUD : public PWidget
{
	PDialogBox* mDialogBox = nullptr;

public:
	PBattleHUD();
	~PBattleHUD() override;

	void Draw(const PRenderer* Renderer) const override;
};