#include "MainGame.h"

#include "Application/Application.h"
#include "Engine/Actors/Interactable.h"
#include "Modes/BattleMode.h"
#include "Modes/MapMode.h"

bool PMainGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}

	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();

	return true;
}

bool PMainGame::Start()
{
	if (!PGame::Start())
	{
		return false;
	}
	mHUD = ConstructWidget<PGameHUD>();
	GWorld->GetRootWidget()->AddChild(mHUD);
	return true;
}

bool PMainGame::HandleGameEvent(SGameEvent& Event)
{
	switch (Event.Type)
	{
		case EGameEventType::Dialog:
			{
				if (!mHUD->IsDialogBoxVisible())
				{
					mHUD->StartDialogBox(Event.GetData<SInteractData>()->Message);
				}
				else
				{
					mHUD->EndDialogBox();
				}
				break;
			}
		case EGameEventType::BattleStart:
			{
				mHUD->EndDialogBox();
				mHUD->StartBattleHUD();
				break;
			}
		case EGameEventType::BattleEnd:
			{
				mHUD->EndBattleHUD();
				break;
			}
		default:
			break;
	}
	return true;
}
