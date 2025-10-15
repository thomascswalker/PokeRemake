#include "BattlePPMenu.h"

#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"

constexpr float BATTLE_PP_MENU_X = 0;
constexpr float BATTLE_PP_MENU_Y = 8;
constexpr float BATTLE_PP_MENU_W = 11;
constexpr float BATTLE_PP_MENU_H = 5;

constexpr float		  BATTLE_PP_MENU_TYPELABEL_X = COORD(1);
constexpr float		  BATTLE_PP_MENU_TYPELABEL_Y = COORD(9);
static const FVector2 BATTLE_PP_MENU_TYPELABEL_POS(BATTLE_PP_MENU_TYPELABEL_X, BATTLE_PP_MENU_TYPELABEL_Y);

constexpr float		  BATTLE_PP_MENU_TYPE_X = COORD(2);
constexpr float		  BATTLE_PP_MENU_TYPE_Y = COORD(10);
static const FVector2 BATTLE_PP_MENU_TYPE_POS(BATTLE_PP_MENU_TYPE_X, BATTLE_PP_MENU_TYPE_Y);

constexpr float		  BATTLE_PP_MENU_REMAINING_X = COORD(5);
constexpr float		  BATTLE_PP_MENU_REMAINING_Y = COORD(11);
static const FVector2 BATTLE_PP_MENU_REMAINING(BATTLE_PP_MENU_REMAINING_X, BATTLE_PP_MENU_REMAINING_Y);

PBattlePPMenu::PBattlePPMenu()
	: PFrame(FRect(BATTLE_PP_MENU_X, BATTLE_PP_MENU_Y, BATTLE_PP_MENU_W, BATTLE_PP_MENU_H))
{
}
void PBattlePPMenu::Draw(const PRenderer* Renderer) const
{
	PFrame::Draw(Renderer);
	TextRenderer::DrawText(u"TYPE/", BATTLE_PP_MENU_TYPELABEL_POS);

	auto Move = GBattleManager->GetSelectedMove();
	TextRenderer::DrawText(GetPokeTypeString(Move->GetDef()->Type), BATTLE_PP_MENU_TYPE_POS);

	std::string PPRemainingText = std::format("{:2}/{}", Move->GetPP(), Move->GetDef()->PP);
	TextRenderer::DrawText(PPRemainingText, BATTLE_PP_MENU_REMAINING);
}
