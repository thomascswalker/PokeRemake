#pragma once

#include "Actors/GameMap.h"

#include "Core/Map.h"

#define MAP_PALLET_TOWN "PalletTown"
#define MAP_PALLET_TOWN_ASH_HOUSE_F0 "PalletTownAshHouseF0"
#define MAP_PALLET_TOWN_ASH_HOUSE_F1 "PalletTownAshHouseF1"
#define MAP_PALLET_TOWN_GARY_HOUSE "PalletTownGaryHouse"

class PMapManager
{
	static TMap<std::string, JSON> sMapData;
	static TMap<std::string, PGameMap*> sActiveMaps;

public:
	static PGameMap* ConstructMap(const JSON& JsonData);
	static PGameMap* GetMap(const std::string& Name);
	static PGameMap* LoadMap(const JSON& Data);
	static PGameMap* LoadMap(const std::string& Name, bool ForceReload);
	static PGameMap* LoadMapFile(const std::string& FileName);

	static bool UnloadMap(const std::string& Name);
	static bool SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition,
	                      EOrientation ExitDirection);

	static PGameMap* GetMapUnderMouse();
	static PGameMap* GetMapAtPosition(const FVector2& Position);
	static void ClearMaps();
};
