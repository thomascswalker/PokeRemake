#pragma once

#include "../Core/Pokemon.h"
#include "../Core/PokeParty.h"
#include "../Core/PokeStorage.h"
#include "Engine/Object.h"

enum class EBattleState
{
	Starting,
	SelectAction = 100,
	SelectMove,
	PlayerAttacking = 200,
	OpponentAttacking,
	Ending = 1000,
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
	PPokemonStorage Storage = {};

	STrainerContext() = default;
	STrainerContext(int32_t InId, const std::string& InName) : Id(InId), Name(InName) { Storage.Clear(); }
	JSON Serialize() const override;
	void Deserialize(const JSON& Json) override;
};

struct SBattleContext
{
	// Pointer to the trainer definition
	STrainerContext* Trainer = nullptr;

	// The opposing party
	PPokemonParty BattleParty;

	// Pointer to the player's current Pokémon
	SPokemon* PlayerMon = nullptr;

	// Pointer to the opponent's current Pokémon
	SPokemon* BattleMon = nullptr;

	// Current battle state
	EBattleState State = EBattleState::Starting;

	// Current action selected
	EBattleAction Action = EBattleAction::Fight;

	// Current move index selected
	uint8_t MoveIndex = 0;

	static JSON Schema()
	{
		return {
			{  "BattleId", nullptr },
			{ "PlayerMon", nullptr },
			{ "BattleMon", nullptr }
		};
	}
};

DECLARE_MULTICAST_DELEGATE(DBattleStateChanged, EBattleState);
DECLARE_MULTICAST_DELEGATE(DBattleActionChanged, EBattleAction);
DECLARE_MULTICAST_DELEGATE(DBattleMoveIndexChanged, uint8_t);

class PBattleManager : public PObject
{
	std::map<int32_t, STrainerContext> mTrainers;
	SBattleContext					   mContext;

public:
	DBattleStateChanged		BattleStateChanged;
	DBattleActionChanged	BattleActionChanged;
	DBattleMoveIndexChanged BattleMoveIndexChanged;

	bool PreStart() override;

	/* Any battle */

	PPokemonParty* GetCurrentBattleParty() { return &mContext.BattleParty; }
	EPartyType	   GetCurrentBattlePartyType() const { return mContext.BattleParty.GetType(); }

	void StartTrainerBattle(int32_t Id);
	void StartWildBattle(int32_t Id, int32_t Level);

	/* Trainers */

	STrainerContext* GetTrainer(int32_t Id);
	int32_t			 GetCurrentTrainerId() const { return mContext.Trainer->Id; }

	PPokemonStorage* GetCurrentTrainerStorage(int32_t Id);
	std::string		 GetCurrentTrainerName() const;

	SPokemon* GetPlayerMon() const
	{
		return mContext.PlayerMon;
	}

	void SetPlayerMon(SPokemon* PlayerMon)
	{
		mContext.PlayerMon = PlayerMon;
	}

	SPokemon* GetBattleMon() const
	{
		return mContext.BattleMon;
	}

	void SetBattleMon(SPokemon* BattleMon)
	{
		mContext.BattleMon = BattleMon;
	}

	void SetBattleMon(int32_t Index)
	{
		mContext.BattleMon = mContext.BattleParty.Get(Index);
	}

	void SwapNextBattleMon();
	void SwapPrevBattleMon();

	EBattleState GetState() const { return mContext.State; }

	void SetState(EBattleState State)
	{
		mContext.State = State;
		BattleStateChanged.Broadcast(State);
	}

	EBattleAction GetAction() const { return mContext.Action; }

	void SetAction(EBattleAction Action)
	{
		mContext.Action = Action;
		BattleActionChanged.Broadcast(Action);
	}

	uint8_t GetMoveIndex() const { return mContext.MoveIndex; }
	void	SetMoveIndex(uint8_t Index)
	{
		mContext.MoveIndex = Index;
		BattleMoveIndexChanged.Broadcast(Index);
	}
};

extern PBattleManager* GBattleManager;