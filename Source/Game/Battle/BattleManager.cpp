#include "BattleManager.h"

#include "Core/GameConstants.h"

PBattleManager* GBattleManager = nullptr;
SPokemon		GWildMon;

JSON STrainerContext::Serialize() const
{
	JSON Result;
	Result["Id"] = Id;
	Result["Dialog"] = Dialog;
	return Result;
}

void STrainerContext::Deserialize(const JSON& Json)
{
	Id = Json["Id"];
	Name = Json["Name"];
	Dialog = Json["Dialog"];
	Storage.Deserialize(Json["Storage"]);
}

bool PBattleManager::PreStart()
{
	std::string BattlesFileName = Files::FindFile(BATTLES_JSON);
	if (BattlesFileName.empty())
	{
		LogError("Failed to find {}.", BattlesFileName.c_str());
		return false;
	}
	std::string Buffer;
	if (!Files::ReadFile(BattlesFileName, Buffer))
	{
		LogError("Failed to read {}.", BattlesFileName.c_str());
		return false;
	}

	JSON Data = JSON::parse(Buffer);

	for (auto& Battle : Data)
	{
		int32_t Id = Battle["Id"];
		mTrainers[Id] = { Battle["Id"], Battle["Name"], Battle["Dialog"] };
		for (auto Mon : Battle["Storage"])
		{
			mTrainers[Id].Storage.Construct(Mon["Id"], Mon["Level"]);
		}
	}

	return true;
}

void PBattleManager::StartTrainerBattle(int32_t Id)
{
	mContext.BattleParty.SetType(PT_Trainer);
	auto BattleParty = &mContext.BattleParty;
	if (mTrainers.contains(Id))
	{
		mContext.Trainer = &mTrainers[Id];
		BattleParty->Clear();
		for (auto& Mon : mTrainers.at(Id).Storage.GetAll())
		{
			BattleParty->Add(&Mon);
		}
	}
	mContext.BattleMon = BattleParty->Get(0);
}

void PBattleManager::StartWildBattle(int32_t Id, int32_t Level)
{
	mContext.BattleParty.SetType(PT_Wild);
	mContext.BattleParty.Clear();
	GWildMon = SPokemon(*PPokedexManager::Instance()->GetMonById(Id), Level, 0);
	mContext.BattleParty.Add(&GWildMon);
	mContext.BattleMon = mContext.BattleParty.Get(0);
}

STrainerContext* PBattleManager::GetTrainer(int32_t Id)
{
	return &mTrainers[Id];
}

PPokemonStorage* PBattleManager::GetCurrentTrainerStorage(int32_t Id)
{
	if (!mTrainers.contains(Id))
	{
		return nullptr;
	}
	return &mTrainers[Id].Storage;
}

std::string PBattleManager::GetCurrentTrainerName() const
{
	if (!mContext.Trainer)
	{
		return "";
	}
	return mContext.Trainer->Name;
}

void PBattleManager::SwapNextBattleMon()
{
	if (mContext.BattleMon == nullptr)
	{
		LogError("No battle mon set.");
		return;
	}

	// If there's only one Pokémon in the party, we can't swap
	auto Count = mContext.BattleParty.GetCount();
	if (Count == 1)
	{
		return;
	}

	int32_t Index = mContext.BattleParty.GetIndex(mContext.BattleMon);
	int32_t NewIndex = Index == Count - 1 ? 0 : Index + 1;
	SetBattleMon(NewIndex);
}
void PBattleManager::SwapPrevBattleMon()
{
	if (mContext.BattleMon == nullptr)
	{
		LogError("No battle mon set.");
		return;
	}

	// If there's only one Pokemon in the party, we can't swap
	auto Count = mContext.BattleParty.GetCount();
	if (Count == 1)
	{
		return;
	}

	int32_t Index = mContext.BattleParty.GetIndex(mContext.BattleMon);
	int32_t NewIndex = Index == 0 ? Count - 1 : Index - 1;
	SetBattleMon(NewIndex);
}
