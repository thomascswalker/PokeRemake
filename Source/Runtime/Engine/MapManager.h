#pragma once

#include "Actors/Map.h"

#define MAP_PALLET_TOWN "PalletTown"
#define MAP_PALLET_TOWN_ASH_HOUSE_F0 "PalletTownAshHouseF0"
#define MAP_PALLET_TOWN_ASH_HOUSE_F1 "PalletTownAshHouseF1"
#define MAP_PALLET_TOWN_GARY_HOUSE "PalletTownGaryHouse"

class PMapManager
{
	static std::map<std::string, JSON> sMapData;

public:
	static PMap* GetMapInWorld(const std::string& Name);
	static PMap* LoadMap(const std::string& Name);
	static bool	 UnloadMap(const std::string& Name);
	static bool	 SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition, EOrientation ExitDirection);
};