#pragma once

#include "../DialogBox.h"
#include "Engine/Dialog.h"
#include "Interface/Widget.h"

#include "BattleActionMenu.h"
#include "BattleMoveMenu.h"

DECLARE_MULTICAST_DELEGATE(DOnDialogComplete);

#define NAME_FONT_SIZE 24

#define SPRITE_SIZE WINDOW_HEIGHT / 2

#define PLAYER_SPRITE_X 0
#define PLAYER_SPRITE_Y SPRITE_SIZE

#define BATTLE_SPRITE_X WINDOW_WIDTH - SPRITE_SIZE
#define BATTLE_SPRITE_Y 0

class PBattleHUD : public PWidget
{
	std::shared_ptr<PSprite> mInterfaceSprite = nullptr;
	PDialogBox*				 mDialogBox = nullptr;
	PBattleActionMenu*		 mActionBox = nullptr;
	PBattleMoveMenu*		 mMoveBox = nullptr;

public:
	DOnDialogComplete OnDialogComplete;

	PBattleHUD();
	~PBattleHUD() override;

	std::array<uint32_t, 6> ComputeHealthBarTiles(SPokemon* Mon) const;

	void DrawTiles(const std::vector<SFrameTile>& Tiles) const;
	void DrawPlayerFrame(const PRenderer* Renderer) const;
	void DrawBattleFrame(const PRenderer* Renderer) const;
	void Draw(const PRenderer* Renderer) const override;

	void ShowActionBox();
	void HideActionBox();

	void ShowDialogBox(const SDialogContext& Context);
	void HideDialogBox();

	void ShowMoveBox();
	void HideMoveBox();
};