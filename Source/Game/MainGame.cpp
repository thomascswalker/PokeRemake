#include "MainGame.h"

#include "Application/Application.h"
#include "Battle/BattleMode.h"
#include "Engine/Actors/Interactable.h"
#include "Modes/MapMode.h"

bool PMainGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}

	// Load all pokemon info
	if (!PPokedexManager::Instance()->Init())
	{
		return false;
	}

	// Construct the main HUD used across all modes (map, battle, storage, etc.)
	mHUD = ConstructWidget<PGameHUD>();

	auto MapMode = AddGameMode<PMapMode>();
	MapMode->PreStart();

	auto BattleMode = AddGameMode<PBattleMode>();
	BattleMode->PreStart();

	// Initialize Player Party
	auto Mon = mPlayerStorage.Construct(ID_BULBASAUR, 5);
	mPlayerParty.Add(Mon);

	return true;
}

bool PMainGame::Start()
{
	if (!PGame::Start())
	{
		return false;
	}
	GWorld->GetRootWidget()->AddChild(mHUD);

	GPlayerParty = &mPlayerParty;
	GPlayerStorage = &mPlayerStorage;

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
					return StartDialogBox(&Event);
				}

				return EndDialogBox();
			}
		case EGameEventType::BattleStart:
			{
				return StartBattle(&Event);
			}
		case EGameEventType::BattleEnd:
			{
				return EndBattle();
			}
		default:
			break;
	}
	return true;
}

bool PMainGame::StartDialogBox(SGameEvent* Event)
{
	SInteractContext* Context = Event->GetData<SInteractContext>();
	if (!Context)
	{
		return false;
	}
	mHUD->StartDialogBox(Context->Message);
	return true;
}

bool PMainGame::EndDialogBox()
{
	mHUD->EndDialogBox();
	return true;
}

bool PMainGame::StartBattle(SGameEvent* Event)
{
	SBattleContext* Context = Event->GetData<SBattleContext>();
	if (!Context)
	{
		return false;
	}
	mHUD->EndDialogBox();
	mHUD->StartBattleHUD();
	return true;
}

bool PMainGame::EndBattle()
{
	mHUD->EndBattleHUD();
	return true;
}
