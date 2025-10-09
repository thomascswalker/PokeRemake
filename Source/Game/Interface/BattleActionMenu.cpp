#include "BattleActionMenu.h"

#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"

#define BAB_X 8

PBattleActionMenu::PBattleActionMenu()
	: PFrame(FRect(BATTLE_ACTION_MENU_X, BATTLE_ACTION_MENU_Y, BATTLE_ACTION_MENU_W, BATTLE_ACTION_MENU_H))
{
}

PBattleActionMenu::~PBattleActionMenu()
{
}

void PBattleActionMenu::Draw(const PRenderer* Renderer) const
{
	// Draw the frame
	PFrame::Draw(Renderer);

	// Set text color
	Renderer->SetDrawColor(0, 0, 0, 255);

	// Draw each action option text
	TextRenderer::DrawText(BATTLE_ACTION_FIGHT, { BATTLE_ACTION_FIGHT_X, BATTLE_ACTION_FIGHT_Y });
	TextRenderer::DrawChar(CHAR_PK, { BATTLE_ACTION_PARTY_X, BATTLE_ACTION_PARTY_Y });
	TextRenderer::DrawChar(CHAR_MN, { BATTLE_ACTION_PARTY_X + COORD(1), BATTLE_ACTION_PARTY_Y });
	TextRenderer::DrawText(BATTLE_ACTION_ITEM, { BATTLE_ACTION_ITEM_X, BATTLE_ACTION_ITEM_Y });
	TextRenderer::DrawText(BATTLE_ACTION_RUN, { BATTLE_ACTION_RUN_X, BATTLE_ACTION_RUN_Y });

	// Draw the selection arrow for the currently-selected action
	switch (GBattleManager->GetSelectedAction())
	{
		case EBattleAction::Fight:
			TextRenderer::DrawChar(CHAR_RIGHT_ARROW, { BATTLE_ACTION_FIGHT_X - COORD(1), BATTLE_ACTION_FIGHT_Y });
			break;
		case EBattleAction::Pokemon:
			TextRenderer::DrawChar(CHAR_RIGHT_ARROW, { BATTLE_ACTION_PARTY_X - COORD(1), BATTLE_ACTION_PARTY_Y });
			break;
		case EBattleAction::Item:
			TextRenderer::DrawChar(CHAR_RIGHT_ARROW, { BATTLE_ACTION_ITEM_X - COORD(1), BATTLE_ACTION_ITEM_Y });
			break;
		case EBattleAction::Run:
			TextRenderer::DrawChar(CHAR_RIGHT_ARROW, { BATTLE_ACTION_RUN_X - COORD(1), BATTLE_ACTION_RUN_Y });
			break;
	}
}
