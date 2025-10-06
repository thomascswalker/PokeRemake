#include "BattleActionBox.h"

#include "Battle/BattleManager.h"
#include "Core/Font.h"
#include "Core/GameConstants.h"

PBattleActionBox::PBattleActionBox()
{
	mTexture = PTextureManager::Get("DialogBox");

	GBattleManager->BattleActionChanged.AddRaw(this, &PBattleActionBox::HandleBattleActionChange);
}

void PBattleActionBox::Draw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(255, 255, 255, 255);
	FRect Geometry = { 0, DIALOG_Y, DIALOG_WIDTH, DIALOG_HEIGHT };

	Renderer->DrawFillRect(Geometry);
	Renderer->DrawTexture(mTexture, mTexture->GetRect(), Geometry);

	Renderer->SetDrawColor(0, 0, 0, 255);

	PokeFont::DrawText(BATTLE_ACTION_FIGHT, { BATTLE_ACTION_FIGHT_X, BATTLE_ACTION_FIGHT_Y });
	PokeFont::DrawChar(CHAR_PK, { BATTLE_ACTION_PARTY_X, BATTLE_ACTION_PARTY_Y });
	PokeFont::DrawChar(CHAR_MN, { BATTLE_ACTION_PARTY_X + COORD(1), BATTLE_ACTION_PARTY_Y });
	PokeFont::DrawText(BATTLE_ACTION_ITEM, { BATTLE_ACTION_ITEM_X, BATTLE_ACTION_ITEM_Y });
	PokeFont::DrawText(BATTLE_ACTION_RUN, { BATTLE_ACTION_RUN_X, BATTLE_ACTION_RUN_Y });

	switch (GBattleManager->GetAction())
	{
		case EBattleAction::Fight:
			PokeFont::DrawText(SELECTION, { BATTLE_ACTION_FIGHT_X - COORD(1), BATTLE_ACTION_FIGHT_Y });
			break;
		case EBattleAction::Pokemon:
			PokeFont::DrawText(SELECTION, { BATTLE_ACTION_PARTY_X - COORD(1), BATTLE_ACTION_PARTY_Y });
			break;
		case EBattleAction::Item:
			PokeFont::DrawText(SELECTION, { BATTLE_ACTION_ITEM_X - COORD(1), BATTLE_ACTION_ITEM_Y });
			break;
		case EBattleAction::Run:
			PokeFont::DrawText(SELECTION, { BATTLE_ACTION_RUN_X - COORD(1), BATTLE_ACTION_RUN_Y });
			break;
	}
}

void PBattleActionBox::HandleBattleActionChange(EBattleAction Action)
{
}