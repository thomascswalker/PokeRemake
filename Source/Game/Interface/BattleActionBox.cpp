#include "BattleActionBox.h"

#include "Battle/BattleManager.h"
#include "Core/GameConstants.h"

PBattleActionBox::PBattleActionBox()
{
	mTexture = PTextureManager::Get("DialogBox");

	GBattleManager->BattleActionChanged.AddRaw(this, &PBattleActionBox::HandleBattleActionChange);
}

void PBattleActionBox::Draw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(0, 0, 0, 255);

	Renderer->DrawText(BATTLE_ACTION_FIGHT, { BATTLE_ACTION_FIGHT_X, BATTLE_ACTION_FIGHT_Y }, DIALOG_FONT_SIZE);
	Renderer->DrawText(BATTLE_ACTION_PARTY, { BATTLE_ACTION_PARTY_X, BATTLE_ACTION_PARTY_Y }, DIALOG_FONT_SIZE);
	Renderer->DrawText(BATTLE_ACTION_ITEM, { BATTLE_ACTION_ITEM_X, BATTLE_ACTION_ITEM_Y }, DIALOG_FONT_SIZE);
	Renderer->DrawText(BATTLE_ACTION_RUN, { BATTLE_ACTION_RUN_X, BATTLE_ACTION_RUN_Y }, DIALOG_FONT_SIZE);
}

void PBattleActionBox::HandleBattleActionChange(EBattleAction Action)
{
	LogInfo("Action: {}", (uint8_t)Action);
}