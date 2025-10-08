#include "BattleActionBox.h"

#include "Battle/BattleManager.h"
#include "Core/Font.h"
#include "Core/GameConstants.h"

#define BAB_X 8

PBattleActionBox::PBattleActionBox()
	: PFrame(FRect(BAB_X, DIALOG_Y, DIALOG_W - BAB_X, DIALOG_H))
{
	GBattleManager->BattleActionChanged.AddRaw(this, &PBattleActionBox::HandleBattleActionChange);
}

PBattleActionBox::~PBattleActionBox()
{
	GBattleManager->BattleActionChanged.RemoveObject(this);
}

void PBattleActionBox::Draw(const PRenderer* Renderer) const
{
	PFrame::Draw(Renderer);

	Renderer->SetDrawColor(0, 0, 0, 255);

	PokeFont::DrawText(BATTLE_ACTION_FIGHT, { BATTLE_ACTION_FIGHT_X, BATTLE_ACTION_FIGHT_Y });
	PokeFont::DrawChar(CHAR_PK, { BATTLE_ACTION_PARTY_X, BATTLE_ACTION_PARTY_Y });
	PokeFont::DrawChar(CHAR_MN, { BATTLE_ACTION_PARTY_X + COORD(1), BATTLE_ACTION_PARTY_Y });
	PokeFont::DrawText(BATTLE_ACTION_ITEM, { BATTLE_ACTION_ITEM_X, BATTLE_ACTION_ITEM_Y });
	PokeFont::DrawText(BATTLE_ACTION_RUN, { BATTLE_ACTION_RUN_X, BATTLE_ACTION_RUN_Y });

	switch (GBattleManager->GetAction())
	{
		case EBattleAction::Fight:
			PokeFont::DrawChar(RIGHT_ARROW, { BATTLE_ACTION_FIGHT_X - COORD(1), BATTLE_ACTION_FIGHT_Y });
			break;
		case EBattleAction::Pokemon:
			PokeFont::DrawChar(RIGHT_ARROW, { BATTLE_ACTION_PARTY_X - COORD(1), BATTLE_ACTION_PARTY_Y });
			break;
		case EBattleAction::Item:
			PokeFont::DrawChar(RIGHT_ARROW, { BATTLE_ACTION_ITEM_X - COORD(1), BATTLE_ACTION_ITEM_Y });
			break;
		case EBattleAction::Run:
			PokeFont::DrawChar(RIGHT_ARROW, { BATTLE_ACTION_RUN_X - COORD(1), BATTLE_ACTION_RUN_Y });
			break;
	}
}

void PBattleActionBox::HandleBattleActionChange(EBattleAction Action)
{
}