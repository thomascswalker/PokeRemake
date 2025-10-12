#include "BattleMode.h"

#include "Application/Application.h"
#include "Core/GameConstants.h"
#include "Engine/Game.h"
#include "Modes/MapMode.h"

#include "MainGame.h"

PBattleMode::PBattleMode()
{
	mState = SGameState(SBattleContext::Schema());
	GBattleManager = &mBattleManager;
}

bool PBattleMode::PreStart()
{
	mBattleManager.PreStart();
	return true;
}

bool PBattleMode::Start()
{
	mBattleManager.Start();
	return true;
}

bool PBattleMode::Load()
{
	SetInputContext(IC_Battle);

	auto Id = mState.Get<int32_t>(STATE_BATTLE_ID);
	mBattleManager.StartTrainerBattle(Id);
	mBattleManager.SetPlayerMon(GPlayerParty->Get(0));

	// Start with Select Action state
	GBattleManager->SetState(EBattleState::SelectAction);

	// Start the main battle HUD
	mHUD = GEngine->GetGameAs<PMainGame>()->GetHUD();
	mHUD->StartBattleHUD();

	return true;
}

bool PBattleMode::Unload()
{
	// Unbind all states related to battle mode
	GBattleManager->BattleActionChanged.RemoveAll();
	GBattleManager->BattleStateChanged.RemoveAll();
	GBattleManager->BattleMoveIndexChanged.RemoveAll();

	// Destroy the battle HUD
	mHUD->EndBattleHUD();

	return true;
}

void PBattleMode::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_Q:
			HandlePressB();
			break;
		case SDLK_E:
			HandlePressA();
			break;
		case SDLK_W:
		case SDLK_A:
		case SDLK_S:
		case SDLK_D:
		case SDLK_RIGHT:
		case SDLK_LEFT:
		case SDLK_DOWN:
		case SDLK_UP:
			HandlePressDPad(RemapKeyToDPad(Event->KeyUp));
			break;
		default:
			break;
	}
}

bool PBattleMode::HandlePressA()
{
	if (GBattleManager->GetState() == EBattleState::SelectAction)
	{
		switch (GBattleManager->GetSelectedAction())
		{
			case EBattleAction::Fight:
				HandleGameEvent({ this, EGameEventType::BattleEnterMove });
				break;
			case EBattleAction::Pokemon:
				LogWarning("Pokemon selection not implemented yet.");
				break;
			case EBattleAction::Item:
				LogWarning("Item selection not implemented yet.");
				break;
			case EBattleAction::Run:
				HandleGameEvent({ this, EGameEventType::BattleEnd });
				break;
		}
	}
	return true;
}

bool PBattleMode::HandlePressB()
{
	switch (GBattleManager->GetState())
	{
		// If on the action menu, and the user presses B
		// exit the battle.
		case EBattleState::SelectAction:
			HandleGameEvent({ this, EGameEventType::BattleEnd });
			break;
		// If on the move menu, and the user presses B,
		// switch to Action mode.
		case EBattleState::SelectMove:
			HandleGameEvent({ this, EGameEventType::BattleExitMove });
			break;
		default:
			break;
	}
	return true;
}

bool PBattleMode::HandlePressDPad(EDPad Direction)
{
	switch (Direction)
	{
		case DPAD_RIGHT:
		case DPAD_DOWN:
		case DPAD_LEFT:
		case DPAD_UP:
			switch (GBattleManager->GetState())
			{
				// If on the action menu, handle the new selection.
				case EBattleState::SelectAction:
					HandleChangeActionSelection(Direction);
					break;
				// If on the move menu, handle the new selection.
				case EBattleState::SelectMove:
					HandleChangeMoveSelection(Direction);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return true;
}

bool PBattleMode::HandleGameEvent(const SGameEvent& GameEvent)
{
	switch (GameEvent.Type)
	{
		case EGameEventType::BattleEnd:
			{
				// Unload this game mode and load Map mode
				GBattleManager->SetSelectedAction(EBattleAction::Fight);
				if (!GEngine->GetGame()->SetAndLoadCurrentGameMode(MAP_MODE))
				{
					return false;
				}
				break;
			}
			// Action menu, selected FIGHT
		case EGameEventType::BattleEnterMove:
			{
				// Default to the first move
				GBattleManager->SetSelectedMoveIndex(0);
				GBattleManager->SetState(EBattleState::SelectMove);
				mHUD->GetBattleHUD()->HideActionBox();
				mHUD->GetBattleHUD()->ShowMoveBox();
				break;
			}
		case EGameEventType::BattleExitMove:
			{
				GBattleManager->SetState(EBattleState::SelectAction);
				mHUD->GetBattleHUD()->HideMoveBox();
				mHUD->GetBattleHUD()->ShowActionBox();
				break;
			}

		default:
			break;
	}
	return false;
}

void PBattleMode::HandleChangeActionSelection(uint8_t Direction)
{
	uint8_t CurrentPosition = static_cast<uint8_t>(GBattleManager->GetSelectedAction());

	bool X = static_cast<bool>(CurrentPosition % 2);
	bool Y = static_cast<bool>(CurrentPosition / 2);

	/*
	 * ------------------------
	 * |    0     |     1     |
	 * -----------|-----------|
	 * |    2     |     3     |
	 * ------------------------
	 */
	switch (Direction)
	{
		case DPAD_RIGHT:
		case DPAD_LEFT:
			X = !X;
			break;
		case DPAD_DOWN:
		case DPAD_UP:
			Y = !Y;
			break;
		default:
			break;
	}

	uint8_t NewX = X;
	uint8_t NewY = Y;

	uint8_t NewPosition = (NewY * 2) + NewX;

	GBattleManager->SetSelectedAction(static_cast<EBattleAction>(NewPosition));
}

void PBattleMode::HandleChangeMoveSelection(EDPad Direction)
{
	if (Direction == DPAD_RIGHT || Direction == DPAD_LEFT)
	{
		return;
	}
	uint8_t Index = GBattleManager->GetSelectedMoveIndex();
	switch (Direction)
	{
		case DPAD_UP:
			Index == 0
				? Index = 3
				: Index--;
			break;
		case DPAD_DOWN:
			Index == 3
				? Index = 0
				: Index++;
			break;
		default:
			break;
	}
	GBattleManager->SetSelectedMoveIndex(Index);
}

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
