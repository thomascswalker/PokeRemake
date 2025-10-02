/**
 * Manages and draws the entire Battle HUD when in Battle Mode.
 */

#include "BattleHUD.h"

PBattleHUD::PBattleHUD()
{
	mDialogBox = ConstructWidget<PDialogBox>();
	PWidget::AddChild(mDialogBox);

	std::string BattleText = "Default text.";
	if (auto BattleMon = GBattleManager->GetBattleMon())
	{
		BattleText = std::format("{} sent out \n{}", GBattleManager->GetCurrentTrainerName().c_str(), BattleMon->GetDisplayName().c_str());
	}
	mDialogBox->SetText(BattleText);
	mDialogBox->Print();
}

PBattleHUD::~PBattleHUD()
{
	mDialogBox->EndPrint();
	mDialogBox->Unparent();
	GWorld->DestroyWidget(mDialogBox);
	mDialogBox = nullptr;
}

void PBattleHUD::Draw(const PRenderer* Renderer) const
{
	// White background
	Renderer->SetDrawColor(255, 255, 255, 255);
	Renderer->DrawFillRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });

	// Draw the player Pokémon
	if (auto Mon = GBattleManager->GetPlayerMon())
	{
		// Draw sprite
		auto  Sprite = Mon->GetBackTexture();
		FRect Rect(PLAYER_MON_X, PLAYER_MON_Y, BATTLE_MON_SIZE, BATTLE_MON_SIZE);
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), Rect);

		// Draw name/level
		Renderer->SetDrawColor(0, 0, 0, 255);
		Renderer->DrawText(Mon->GetDisplayName(), { PLAYER_ORIGIN_X, PLAYER_ORIGIN_Y }, NAME_FONT_SIZE);
		Renderer->DrawText(Mon->GetDisplayLevel(), { PLAYER_LEVEL_X, PLAYER_LEVEL_Y }, NAME_FONT_SIZE);
	}

	// Draw the Pokémon being battled
	if (auto Mon = GBattleManager->GetBattleMon())
	{
		// Draw sprite
		auto  Sprite = Mon->GetFrontTexture();
		FRect Rect(BATTLE_MON_X, BATTLE_MON_Y, BATTLE_MON_SIZE, BATTLE_MON_SIZE);
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), Rect);

		// Draw name/level
		Renderer->SetDrawColor(0, 0, 0, 255);
		Renderer->DrawText(Mon->GetDisplayName(), { BATTLE_ORIGIN_X, BATTLE_ORIGIN_Y }, NAME_FONT_SIZE);
		Renderer->DrawText(Mon->GetDisplayLevel(), { BATTLE_LEVEL_X, BATTLE_LEVEL_Y }, NAME_FONT_SIZE);
	}
}