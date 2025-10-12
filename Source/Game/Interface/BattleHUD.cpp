/**
 * Manages and draws the entire Battle HUD when in Battle Mode.
 */

#include "BattleHUD.h"

#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"

constexpr auto	   CHAR_LEVEL = u":Ł"; // Ł
constexpr float	   LEVEL_SCALE = 0.5f;
constexpr float	   BATTLE_MON_SIZE = COORD(12);
static const FRect PLAYER_MON_RECT = {
	COORD(-1),
	COORD(3),
	BATTLE_MON_SIZE,
	BATTLE_MON_SIZE,
};
static const FRect BATTLE_MON_RECT = {
	COORD(10),
	COORD(-3),
	BATTLE_MON_SIZE,
	BATTLE_MON_SIZE,
};

constexpr float PLAYER_ORIGIN_X = COORD(10);
constexpr float PLAYER_ORIGIN_Y = COORD(7);
constexpr float BATTLE_ORIGIN_X = COORD(1);
constexpr float BATTLE_ORIGIN_Y = COORD(0);

constexpr float LEVEL_X = COORD(3);
constexpr float LEVEL_Y = COORD(1);

PBattleHUD::PBattleHUD()
{
	mDialogBox = ConstructWidget<PDialogBox>();

	std::string BattleText = "Default text.";
	if (auto BattleMon = GBattleManager->GetBattleMon())
	{
		BattleText = std::format("{} sent out \n{}", GBattleManager->GetCurrentTrainerName().c_str(), BattleMon->GetDisplayName().c_str());
	}

	// Background dialog box
	PWidget::AddChild(ConstructWidget<PDialogBox>());

	mDialogBox->SetText(BattleText);
	mDialogBox->Print();

	mActionBox = ConstructWidget<PBattleActionMenu>();
	mMoveBox = ConstructWidget<PBattleMoveMenu>();
}

PBattleHUD::~PBattleHUD()
{
	mDialogBox->EndPrint();
	mDialogBox->Unparent();
	GWorld->DestroyWidget(mDialogBox);
	mDialogBox = nullptr;

	mActionBox->Unparent();
	GWorld->DestroyWidget(mActionBox);
	mActionBox = nullptr;

	mMoveBox->Unparent();
	GWorld->DestroyWidget(mMoveBox);
	mMoveBox = nullptr;
}

void PBattleHUD::Draw(const PRenderer* Renderer) const
{
	// White background
	Renderer->SetDrawColor(255, 255, 255, 255);
	Renderer->DrawFillRect({ 0, 0, WINDOW_PXL_WIDTH, WINDOW_PXL_HEIGHT });

	// Draw the player Pokémon
	if (auto Mon = GBattleManager->GetPlayerMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetBackTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), PLAYER_MON_RECT);

		// Draw name/level
		Renderer->SetDrawColor(0, 0, 0, 255);
		TextRenderer::DrawText(Mon->GetDisplayName(), { PLAYER_ORIGIN_X, PLAYER_ORIGIN_Y });
		TextRenderer::DrawText(CHAR_LEVEL, { PLAYER_ORIGIN_X + LEVEL_X - COORD(LEVEL_SCALE), PLAYER_ORIGIN_Y + LEVEL_Y + COORD(LEVEL_SCALE / 2) }, LEVEL_SCALE);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { PLAYER_ORIGIN_X + LEVEL_X + COORD(1), PLAYER_ORIGIN_Y + LEVEL_Y });
	}

	// Draw the Pokémon being battled
	if (auto Mon = GBattleManager->GetBattleMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetFrontTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), BATTLE_MON_RECT);

		// Draw name/level
		Renderer->SetDrawColor(0, 0, 0, 255);
		TextRenderer::DrawText(Mon->GetDisplayName(), { BATTLE_ORIGIN_X, BATTLE_ORIGIN_Y });
		TextRenderer::DrawText(CHAR_LEVEL, { BATTLE_ORIGIN_X + LEVEL_X - COORD(1 / LEVEL_SCALE), BATTLE_ORIGIN_Y + LEVEL_Y + COORD(LEVEL_SCALE / 2) }, LEVEL_SCALE);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { BATTLE_ORIGIN_X + LEVEL_X, BATTLE_ORIGIN_Y + LEVEL_Y });
	}
}

void PBattleHUD::ShowActionBox()
{
	mActionBox->SetVisible(true);
	PWidget::AddChild(mActionBox);
}

void PBattleHUD::HideActionBox()
{

	mActionBox->SetVisible(false);
	PWidget::RemoveChild(mActionBox);
}

void PBattleHUD::ShowDialogBox()
{
	mDialogBox->SetVisible(true);
	PWidget::AddChild(mDialogBox);
}

void PBattleHUD::HideDialogBox()
{
	mDialogBox->SetVisible(false);
	PWidget::RemoveChild(mDialogBox);
}

void PBattleHUD::ShowMoveBox()
{
	mMoveBox->SetVisible(true);
	PWidget::AddChild(mMoveBox);
}

void PBattleHUD::HideMoveBox()
{
	mMoveBox->SetVisible(false);
	PWidget::RemoveChild(mMoveBox);
}