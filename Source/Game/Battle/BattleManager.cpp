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
	mBattleParty.SetType(PT_Trainer);
	if (mTrainers.contains(Id))
	{
		mTrainer = &mTrainers[Id];
		mBattleParty.Clear();
		for (auto& Mon : mTrainers.at(Id).Storage.GetAll())
		{
			mBattleParty.Add(&Mon);
		}
	}
	mBattleMon = mBattleParty.Get(0);
}

void PBattleManager::StartWildBattle(int32_t Id, int32_t Level)
{
	mBattleParty.SetType(PT_Wild);
	mBattleParty.Clear();
	GWildMon = SPokemon(*PPokedexManager::Instance()->GetMonById(Id), Level, 0);
	mBattleParty.Add(&GWildMon);
	mBattleMon = mBattleParty.Get(0);
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
	if (!mTrainer)
	{
		return "";
	}
	return mTrainer->Name;
}

void PBattleManager::SwapNextBattleMon()
{
	if (mBattleMon == nullptr)
	{
		LogError("No battle mon set.");
		return;
	}

	// If there's only one Pokémon in the party, we can't swap
	auto Count = mBattleParty.GetCount();
	if (Count == 1)
	{
		return;
	}

	int32_t Index = mBattleParty.GetIndex(mBattleMon);
	int32_t NewIndex = Index == Count - 1 ? 0 : Index + 1;
	SetBattleMon(NewIndex);
}

void PBattleManager::SwapPrevBattleMon()
{
	if (mBattleMon == nullptr)
	{
		LogError("No battle mon set.");
		return;
	}

	// If there's only one Pokémon in the party, we can't swap
	auto Count = mBattleParty.GetCount();
	if (Count == 1)
	{
		return;
	}

	int32_t Index = mBattleParty.GetIndex(mBattleMon);
	int32_t NewIndex = Index == 0 ? Count - 1 : Index - 1;
	SetBattleMon(NewIndex);
}
