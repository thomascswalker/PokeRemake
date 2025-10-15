/**
 * Manages and draws the entire Battle HUD when in Battle Mode.
 */

#include "BattleHUD.h"

#include "Core/GameConstants.h"
#include "Core/TextRenderer.h"

#include "BattleInterfaceConstants.h"

PBattleHUD::PBattleHUD()
{
	mInterfaceSprite = std::make_shared<PSprite>();
	mInterfaceSprite->SetTexture(GTextureManager->Get("Interface"));

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

void PBattleHUD::DrawPlayerFrame(const PRenderer* Renderer) const
{
	// Draw the player Pokémon
	if (auto Mon = GBattleManager->GetPlayerMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetBackTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), PLAYER_MON_RECT);

		// Draw name/level
		TextRenderer::DrawText(Mon->GetDisplayName(), { PLAYER_ORIGIN_X, PLAYER_ORIGIN_Y });
		Renderer->DrawSprite(mInterfaceSprite.get(), { PLAYER_LEVEL_X, PLAYER_LEVEL_Y, COORD(1), COORD(1) }, UI_INDEX_LEVEL);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { PLAYER_LEVEL_X + COORD(1), PLAYER_LEVEL_Y });

		// Draw HP text
		auto	HpText = Mon->GetDisplayHp();
		int32_t HpTextLength = HpText.size();
		int32_t HpXOffset = MAX_HP_TEXT_LENGTH - HpTextLength;
		TextRenderer::DrawText(HpText, { PLAYER_ORIGIN_X + COORD(1) + COORD(HpXOffset), PLAYER_ORIGIN_Y + COORD(3) });
	}
}

void PBattleHUD::DrawBattleFrame(const PRenderer* Renderer) const
{
	// Draw the Pokémon being battled
	if (auto Mon = GBattleManager->GetBattleMon())
	{
		// Draw sprite
		auto Sprite = Mon->GetFrontTexture();
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), BATTLE_MON_RECT);

		// Draw name/level
		TextRenderer::DrawText(Mon->GetDisplayName(), { BATTLE_ORIGIN_X, BATTLE_ORIGIN_Y });
		Renderer->DrawSprite(mInterfaceSprite.get(), { BATTLE_LEVEL_X - COORD(1), BATTLE_LEVEL_Y, COORD(1), COORD(1) }, UI_INDEX_LEVEL);
		TextRenderer::DrawText(Mon->GetDisplayLevel(), { BATTLE_LEVEL_X, BATTLE_LEVEL_Y });
	}
}

void PBattleHUD::Draw(const PRenderer* Renderer) const
{
	// White background
	Renderer->SetDrawColor(255, 255, 255, 255);
	Renderer->DrawFillRect({ 0, 0, WINDOW_PXL_WIDTH, WINDOW_PXL_HEIGHT });

	DrawPlayerFrame(Renderer);
	DrawBattleFrame(Renderer);
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