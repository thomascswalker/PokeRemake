#include "BattleManager.h"

#define DEFINE_BATTLE_MON(Id, MonId, Level) mBattles[Id].Construct(MonId, Level)

PBattleManager* GBattleManager = nullptr;

bool PBattleManager::Start()
{
	// Gary, Oak lab
	{
		DEFINE_BATTLE_MON(BATTLE_ID_GARY_OAK_LAB, ID_SQUIRTLE, 5);
		DEFINE_BATTLE_MON(BATTLE_ID_GARY_OAK_LAB, ID_CHARMANDER, 5);
	}
	return true;
}