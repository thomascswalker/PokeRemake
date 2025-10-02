#include "BattleManager.h"

#include "Core/GameConstants.h"

PBattleManager* GBattleManager = nullptr;

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
		mBattles[Id] = { .Id = Battle["Id"], .Name = Battle["Name"] };
		for (auto Mon : Battle["Mons"])
		{
			mBattles[Id].Storage.Construct(Mon["Id"], Mon["Level"]);
		}
	}

	return true;
}

SBattle* PBattleManager::GetBattle(int32_t Id)
{
	return &mBattles[Id];
}

void PBattleManager::SetCurrentBattleId(int32_t Id)
{
	if (mBattles.contains(Id))
	{
		mContext.Battle = &mBattles[Id];
		mBattleParty.Clear();
		for (auto& Mon : mBattles.at(Id).Storage.GetAll())
		{
			mBattleParty.Add(&Mon);
		}
	}
	mContext.BattleMon = mBattleParty.Get(0);
}
PPokemonStorage* PBattleManager::GetCurrentBattleStorage(int32_t Id)
{
	if (!mBattles.contains(Id))
	{
		return nullptr;
	}
	return &mBattles[Id].Storage;
}
std::string PBattleManager::GetCurrentBattleName() const
{
	if (!mContext.Battle)
	{
		return "";
	}
	return mContext.Battle->Name;
}
void PBattleManager::NextBattleMon()
{
	if (mContext.BattleMon == nullptr)
	{
		LogError("No battle mon set.");
		return;
	}
	int32_t Index = mBattleParty.GetIndex(mContext.BattleMon);
	int32_t NewIndex = Index == mBattleParty.GetCount() - 1 ? 0 : Index + 1;
	SetBattleMon(NewIndex);
}
