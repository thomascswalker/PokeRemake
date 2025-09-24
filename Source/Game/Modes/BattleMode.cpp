#include "BattleMode.h"

#include "../Battle/BattleView.h"
#include "Application/Application.h"
#include "Battle/BattleHUD.h"
#include "Battle/BattleManager.h"
#include "Engine/Game.h"
#include "Interface/GameHUD.h"

PBattleMode::PBattleMode()
{
	mBattleHUD = ConstructWidget<PBattleHUD>();
}

bool PBattleMode::Load()
{
	SetInputContext(IC_Battle);

	mBattleView = ConstructActor<PBattleView>();
	mBattleView->Start();

	auto HUD = GetHUD();
	if (!HUD)
	{
		LogError("HUD is invalid.");
		return false;
	}
	HUD->RemoveAllChildren();
	HUD->AddChild(mBattleHUD);

	return true;
}

bool PBattleMode::Unload()
{
	GetWorld()->DestroyAllActors();
	return true;
}

void PBattleMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			{

				GetGame()->SetAndLoadCurrentGameMode("MapMode");
				break;
			}
		default:
			break;
	}
}

void PBattleMode::StartBattle(const SBattleEvent& Event)
{

	PBattleManager::Instance()->StartBattle(Event);
}

void PBattleMode::EndBattle()
{
	PBattleManager::Instance()->EndBattle();
}
