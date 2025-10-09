#include "BattleMoveMenu.h"

#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"
#include "Engine/World.h"

#include "BattlePPMenu.h"

inline std::array<std::string, MAX_BATTLE_MOVES> GMoves = { "TACKLE", "GROWL", "", "" };

PBattleMoveMenu::PBattleMoveMenu()
	: PFrame(FRect(BATTLE_MOVE_MENU_X, BATTLE_MOVE_MENU_Y, BATTLE_MOVE_MENU_W, BATTLE_MOVE_MENU_H))
{
	PWidget::AddChild(ConstructWidget<PBattlePPMenu>());
}

void PBattleMoveMenu::Draw(const PRenderer* Renderer) const
{
	PFrame::Draw(Renderer);

	for (uint32_t Index = 0; Index < MAX_BATTLE_MOVES; Index++)
	{
		std::string Text = GMoves[Index].empty() ? "-" : GMoves[Index];
		float		MoveY = BATTLE_MOVE_MENU_TEXT_Y + COORD(Index);
		TextRenderer::DrawText(Text, FVector2(BATTLE_MOVE_MENU_TEXT_X, MoveY));

		if (GBattleManager->GetSelectedMove() == Index)
		{
			TextRenderer::DrawChar(CHAR_RIGHT_ARROW, FVector2(BATTLE_MOVE_SELECTION_X, MoveY));
		}
	}
}