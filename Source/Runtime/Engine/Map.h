#pragma once
#include "Actors/Chunk.h"

#include <string>

#define MAP_PALLET_TOWN "PalletTown"
#define MAP_PALLET_TOWN_ASH_HOUSE_F0 "PalletTownAshHouseF0"
#define MAP_PALLET_TOWN_ASH_HOUSE_F1 "PalletTownAshHouseF1"
#define MAP_PALLET_TOWN_GARY_HOUSE "PalletTownGaryHouse"

class PMapManager
{
public:
	static PChunk* GetMapInWorld(const std::string& Name);
	static PChunk* LoadMap(const std::string& Name);
	static bool	   UnloadMap(const std::string& Name);
	static bool	   SwitchMap(const std::string& OldMap, const std::string& NewMap, const IVector2& NewPosition);
};