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
	switch (GBattleManager->GetState())
	{
		case EBattleState::SelectAction:
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
			break;
		case EBattleState::SelectMove:
			HandleGameEvent({ this, EGameEventType::BattleUseMove });
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
		case EGameEventType::BattleUseMove:
			{
				auto CurrentMove = GBattleManager->GetSelectedMove();
				if (CurrentMove->GetPP() > 0)
				{
					HandleUseMove(CurrentMove, GBattleManager->GetPlayerMon(), GBattleManager->GetBattleMon());
				}
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

	uint8_t NewPosition = NewY * 2 + NewX;

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

uint32_t PBattleMode::ComputeDamage(SPokemonMove* Move, const SPokemon* Attacker, const SPokemon* Target)
{
	// https://bulbapedia.bulbagarden.net/wiki/Damage#Generation_I

	// Compute base damage
	uint32_t LevelFactor = 2 * Attacker->GetLevel() / 5 + 2;
	uint32_t PowerFactor = Move->GetDef()->Power;
	uint32_t StatFactor = Attacker->GetAttack() / Target->GetDefense();
	uint32_t BaseDamage = LevelFactor * PowerFactor * StatFactor / 50 + 2;

	// Compute same type effectiveness (STAT). This is 1.5 if the Move is any of the Target's types.
	// It is 1 otherwise.
	float SameTypeEffectiveness = Target->IsAnyType(Move->GetDef()->Type) ? 1.5f : 1.0f;

	// TODO: Add type effectiveness matrix.

	return BaseDamage * SameTypeEffectiveness;
}

void PBattleMode::HandleUseMove(SPokemonMove* Move, SPokemon* Attacker, SPokemon* Target)
{
	// TODO: Handle non-power (status, healing, etc.) moves.

	LogInfo("{} used {}", Attacker->GetDisplayName().c_str(), Move->GetDef()->Name.c_str());

	uint32_t DamageDealt = ComputeDamage(Move, Attacker, Target);
	LogInfo("{} damage dealt.", DamageDealt);

	uint32_t RemainingHp = Target->Damage(DamageDealt);

	LogInfo("{} has {} HP remaining.", Target->GetDisplayName().c_str(), RemainingHp);
}

std::string PBattleMode::GetName()
{
	return BATTLE_MODE;
}
