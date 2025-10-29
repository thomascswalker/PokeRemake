#pragma once

#include "../Core/GameConstants.h"
#include "../Core/Pokemon.h"
#include "../Core/PokeParty.h"
#include "../Core/PokeStorage.h"
#include "Engine/Object.h"

enum class EBattleState
{
	BattleIntro,
	PlayerActionMenu,
	PlayerMoveMenu,
	EnemyTurn,
	ExecutingMoves,
	BattleEnd,
	Victory,
	Defeat
};

enum class ETurnState
{
	AttackerMoveDialog,
	AttackerMove,
	AttackerDamageDialog,
	TargetMoveDialog,
	TargetMove,
	TargetDamageDialog,
};

enum class EBattleAction
{
	Fight,
	Pokemon,
	Item,
	Run,
};

struct STrainerContext : ISerializable
{
	int32_t			Id = 0;
	std::string		Name = "";
	std::string		Dialog = "";
	PPokemonStorage Storage = {};

	STrainerContext() = default;
	STrainerContext(int32_t InId, const std::string& InName, const std::string& InDialog)
		: Id(InId), Name(InName), Dialog(InDialog)
	{
		Storage.Clear();
	}

	JSON Serialize() const override;
	void Deserialize(const JSON& Json) override;
};

DECLARE_MULTICAST_DELEGATE(DBattleStateChanged, EBattleState);
DECLARE_MULTICAST_DELEGATE(DBattleActionChanged, EBattleAction);
DECLARE_MULTICAST_DELEGATE(DBattleMoveIndexChanged, uint8_t);

class PBattleManager : public PObject
{
	std::map<int32_t, STrainerContext> mTrainers;

	// Pointer to the trainer definition
	STrainerContext* mTrainer = nullptr;

	// The opposing party
	PPokemonParty mBattleParty;

	// Pointer to the player's current Pokémon
	SPokemon* mPlayerMon = nullptr;

	// Pointer to the opponent's current Pokémon
	SPokemon* mBattleMon = nullptr;

	// Current battle state
	EBattleState mState = EBattleState::BattleIntro;

	// Current action selected
	EBattleAction mAction = EBattleAction::Fight;

	// Current move index selected
	uint8_t mMoveIndex = 0;

public:
	DBattleStateChanged		BattleStateChanged;
	DBattleActionChanged	BattleActionChanged;
	DBattleMoveIndexChanged BattleMoveIndexChanged;

	bool PreStart() override;

	/* Any battle */

	PPokemonParty* GetCurrentBattleParty() { return &mBattleParty; }
	EPartyType	   GetCurrentBattlePartyType() const { return mBattleParty.GetType(); }

	void StartTrainerBattle(int32_t Id);
	void StartWildBattle(int32_t Id, int32_t Level);

	/* Trainers */

	STrainerContext* GetTrainer(int32_t Id);
	int32_t			 GetCurrentTrainerId() const { return mTrainer->Id; }

	PPokemonStorage* GetCurrentTrainerStorage(int32_t Id);
	std::string		 GetCurrentTrainerName() const;

	SPokemon* GetPlayerMon() const
	{
		return mPlayerMon;
	}

	void SetPlayerMon(SPokemon* PlayerMon)
	{
		mPlayerMon = PlayerMon;
	}

	SPokemon* GetBattleMon() const
	{
		return mBattleMon;
	}

	void SetBattleMon(SPokemon* BattleMon)
	{
		mBattleMon = BattleMon;
	}

	void SetBattleMon(int32_t Index)
	{
		mBattleMon = mBattleParty.Get(Index);
	}

	void SwapNextBattleMon();
	void SwapPrevBattleMon();

	EBattleState GetState() const { return mState; }

	void SetState(EBattleState State)
	{
		mState = State;
		BattleStateChanged.Broadcast(State);
	}

	EBattleAction GetSelectedAction() const { return mAction; }

	void SetSelectedAction(EBattleAction Action)
	{
		mAction = Action;
		BattleActionChanged.Broadcast(Action);
	}

	SPokemonMove* GetSelectedMove() const { return mPlayerMon->GetMove(mMoveIndex); }
	uint8_t		  GetSelectedMoveIndex() const { return mMoveIndex; }
	void		  SetSelectedMoveIndex(uint8_t Index)
	{
		mMoveIndex = Index;
		BattleMoveIndexChanged.Broadcast(Index);
	}
	// TODO: Update this to reflect actual move count of the currently-selected player's Pokemon.
	uint8_t GetMoveCount() const { return MAX_BATTLE_MOVES; }
};

extern PBattleManager* GBattleManager;