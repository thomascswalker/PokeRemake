#include "BattleMoveMenu.h"

#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"
#include "Engine/World.h"

#include "BattlePPMenu.h"

inline std::array<uint32_t, MAX_BATTLE_MOVES> GMoves = {
	MOVE_TACKLE,
	MOVE_GROWL,
	MOVE_FLAMETHROWER,
	MOVE_HYPER_BEAM,
};

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
		auto  Move = GBattleManager->GetPlayerMon()->GetMove(Index);
		auto  MoveText = Move != nullptr ? Move->GetDef()->Name : "-";
		float MoveY = BATTLE_MOVE_MENU_TEXT_Y + COORD(Index);
		TextRenderer::DrawText(Strings::ToUpper(MoveText), FVector2(BATTLE_MOVE_MENU_TEXT_X, MoveY));

		if (GBattleManager->GetSelectedMove() == Index)
		{
			TextRenderer::DrawText(CHAR_RIGHT_ARROW, FVector2(BATTLE_MOVE_SELECTION_X, MoveY));
		}
	}
}