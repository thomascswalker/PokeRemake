#include "MainGame.h"

#include "Application/Application.h"
#include "Battle/BattleMode.h"
#include "Core/Font.h"
#include "Core/GameSerializer.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Dialog.h"
#include "Modes/MapMode.h"

PMainGame::PMainGame()
{
	mSerializer = std::make_shared<PGameSerializer>();
	GSerializer = mSerializer.get();
}

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
	auto Mon = mPlayerStorage.Construct(ID_CHARMANDER, 5);
	mPlayerParty.Add(Mon);

	LogInfo("Game PreStart successful.");

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

	PokeFont::LoadFontData();

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
	mHUD->StartDialogBox(Event->GetData<SDialogContext>());
	return true;
}

bool PMainGame::EndDialogBox()
{
	mHUD->EndDialogBox();
	return true;
}

bool PMainGame::StartBattle(SGameEvent* Event)
{
	mHUD->EndDialogBox();
	mHUD->StartBattleHUD();
	return true;
}

bool PMainGame::EndBattle()
{
	mHUD->EndBattleHUD();
	return true;
}

void PMainGame::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_F1:
			GSettings->DebugDraw = !GSettings->DebugDraw;
			LogDebug("Debug mode: {}", GSettings->DebugDraw ? "ON" : "OFF");
			break;
		default:
			break;
	}
}
