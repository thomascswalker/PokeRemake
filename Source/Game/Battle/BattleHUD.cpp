#include "BattleHUD.h"

PBattleHUD::PBattleHUD()
{
	mDialogBox = ConstructWidget<PDialogBox>();
	PWidget::AddChild(mDialogBox);

	std::string BattleText = "Default text.";
	if (auto BattleMon = GBattleManager->GetBattleMon())
	{
		BattleText = std::format("{} sent out \n{}", GBattleManager->GetCurrentBattleName().c_str(), BattleMon->GetDisplayName().c_str());
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
	Renderer->DrawFillRect({ 0, 0, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT });

	// Draw the player pokemon
	if (auto PlayerMon = GBattleManager->GetPlayerMon())
	{
		auto	 Sprite = PlayerMon->GetBackTexture();
		FVector2 Position = { PLAYER_SPRITE_X, PLAYER_SPRITE_Y - 100 /* Height of dialog box */ + 50 /* Offset within the sprite */ };
		FRect	 Rect(Position, FVector2(SPRITE_SIZE, SPRITE_SIZE));
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), Rect);

		Renderer->SetDrawColor(255, 0, 0, 255);
		Renderer->DrawRect(Rect);
	}

	// Draw the pokemon being battled
	if (auto BattleMon = GBattleManager->GetBattleMon())
	{
		auto	 Sprite = BattleMon->GetFrontTexture();
		FVector2 Position = { BATTLE_SPRITE_X, BATTLE_SPRITE_Y };
		FRect	 Rect(Position, FVector2(SPRITE_SIZE, SPRITE_SIZE));
		Renderer->DrawTexture(Sprite, Sprite->GetRect(), Rect);

		Renderer->SetDrawColor(255, 0, 0, 255);
		Renderer->DrawRect(Rect);
	}
}