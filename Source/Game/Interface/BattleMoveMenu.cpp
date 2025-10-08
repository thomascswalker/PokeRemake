#include "BattleMoveMenu.h"

#include "Core/Font.h"
#include "Core/GameConstants.h"

inline std::array<std::string, 4> GMoves = { "TACKLE", "GROWL", "", "" };

PBattleMoveMenu::PBattleMoveMenu()
	: PFrame(FRect(BATTLE_MOVE_MENU_X, BATTLE_MOVE_MENU_Y, BATTLE_MOVE_MENU_W, BATTLE_MOVE_MENU_H))
{
}

void PBattleMoveMenu::Draw(const PRenderer* Renderer) const
{
	PFrame::Draw(Renderer);

	float OriginX = COORD(6);
	float OriginY = COORD(13);

	for (uint32_t Index = 0; Index < GMoves.size(); Index++)
	{
		std::string Text = GMoves[Index].empty() ? "?" : GMoves[Index];
		float		MoveY = OriginY + COORD(Index);
		PokeFont::DrawText(Text, FVector2(OriginX, MoveY));
	}
}