#include "BattleMode.h"

#include "Application/Application.h"
#include "Core/GameConstants.h"
#include "Engine/Game.h"
#include "Modes/MapMode.h"

#include "BattleEvents.h"
#include "MainGame.h"

PBattleMode::PBattleMode()
{
	mState = SGameState();
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
	GBattleManager->SetState(EBattleState::PlayerActionMenu);

	// Full heal all opponent mons
	for (auto Mon : GBattleManager->GetCurrentBattleParty()->GetMons())
	{
		Mon->FullHeal();
	}

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
	switch (GBattleManager->GetState())
	{
		// If on the action menu, switch state to the currently-selected
		// action.
		case EBattleState::PlayerActionMenu:
			switch (GBattleManager->GetSelectedAction())
			{
				case EBattleAction::Fight:
					EnterMoveSelection();
					break;
				case EBattleAction::Pokemon:
					LogWarning("Pokemon selection not implemented yet.");
					break;
				case EBattleAction::Item:
					LogWarning("Item selection not implemented yet.");
					break;
				case EBattleAction::Run:
					EndBattle();
					break;
			}
			break;

		// When the player chooses a move, begin executing this
		// turn cycle. The player goes first, then the opponent, then
		// we reset to the player action menu.
		case EBattleState::PlayerMoveMenu:
			ExecuteTurn();
			break;
		default:
			break;
	}
	return true;
}

bool PBattleMode::HandlePressB()
{
	switch (GBattleManager->GetState())
	{
		// If on the action menu, and the user presses B
		// exit the battle.
		case EBattleState::PlayerActionMenu:
			EndBattle();
			break;
		// If on the move menu, and the user presses B,
		// switch to Action mode.
		case EBattleState::PlayerMoveMenu:
			ExitMoveSelection();
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
				case EBattleState::PlayerActionMenu:
					ChangeActionSelection(Direction);
					break;
				// If on the move menu, handle the new selection.
				case EBattleState::PlayerMoveMenu:
					ChangeMoveSelection(Direction);
				default:
					break;
			}
			break;
		default:
			break;
	}
	return true;
}

void PBattleMode::EndBattle() const
{
	GBattleManager->SetSelectedAction(EBattleAction::Fight);
	GEngine->GetGame()->SetAndLoadCurrentGameMode(MAP_MODE);
}

void PBattleMode::EnterMoveSelection()
{
	GBattleManager->SetSelectedMoveIndex(0);
	GBattleManager->SetState(EBattleState::PlayerMoveMenu);
	mHUD->GetBattleHUD()->HideActionBox();
	mHUD->GetBattleHUD()->ShowMoveBox();
}

void PBattleMode::ExitMoveSelection()
{
	GBattleManager->SetState(EBattleState::PlayerActionMenu);
	mHUD->GetBattleHUD()->HideMoveBox();
	mHUD->GetBattleHUD()->ShowActionBox();
}

void PBattleMode::DealDamage(SPokemonMove* Move, const SPokemon* Attacker, SPokemon* Target) const
{
	LogInfo("{} used {}", Attacker->GetDisplayName().c_str(), Move->GetDef()->Name.c_str());

	uint32_t DamageDealt = ComputeDamage(Move, Attacker, Target);
	LogInfo("{} damage dealt.", DamageDealt);

	uint32_t RemainingHp = Target->Damage(DamageDealt);

	LogInfo("{} has {} HP remaining.", Target->GetDisplayName().c_str(), RemainingHp);
}

void PBattleMode::ExecuteTurn() const
{
	LogInfo("Executing turn.");
	GBattleManager->SetState(EBattleState::ExecutingMoves);

	// Player move

	auto PlayerMove = GBattleManager->GetSelectedMove();
	auto PlayerMon = GBattleManager->GetPlayerMon();
	auto TargetMon = GBattleManager->GetBattleMon();

	uint32_t DamageDealt = ComputeDamage(PlayerMove, PlayerMon, TargetMon);
	uint32_t RemainingHp = TargetMon->Damage(DamageDealt);

	if (RemainingHp == 0)
	{
		// Battle won
		GBattleManager->SetState(EBattleState::Victory);
		EndBattle();
		return;
	}

	// Target move

	auto TargetMove = TargetMon->GetMove(0); // Just get the first move
	DamageDealt = ComputeDamage(TargetMove, TargetMon, PlayerMon);
	RemainingHp = PlayerMon->Damage(DamageDealt);

	if (RemainingHp == 0)
	{
		// Battle won
		GBattleManager->SetState(EBattleState::Defeat);
		EndBattle();
		return;
	}

	// Reset to the action menu
	EndTurn();
}

void PBattleMode::EndTurn() const
{
	GBattleManager->SetState(EBattleState::PlayerActionMenu);
	GBattleManager->SetSelectedAction(EBattleAction::Fight);
	mHUD->GetBattleHUD()->HideDialogBox();
	mHUD->GetBattleHUD()->HideMoveBox();
	mHUD->GetBattleHUD()->ShowActionBox();
}

void PBattleMode::ChangeActionSelection(uint8_t Direction)
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

	uint8_t NewPosition = NewY * 2 + NewX;

	GBattleManager->SetSelectedAction(static_cast<EBattleAction>(NewPosition));
}

void PBattleMode::ChangeMoveSelection(EDPad Direction)
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

//
// void PBattleMode::HandleOpponentMonFaint(SPokemon* Mon)
// {
// 	LogInfo("{} fainted.", Mon->GetDisplayName().c_str());
// 	auto Party = GBattleManager->GetCurrentBattleParty();
// 	if (auto NextMon = Party->GetNextUsableMon())
// 	{
// 		// Swap
// 		GBattleManager->SetBattleMon(NextMon);
// 	}
// 	else
// 	{
// 		// win battle
// 		LogInfo("Battle won with {}", GBattleManager->GetCurrentTrainerName().c_str());
// 		EndBattle();
// 	}
// }

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
