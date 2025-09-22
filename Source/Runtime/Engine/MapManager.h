#pragma once

#include "Actors/GameMap.h"
#include "Core/Map.h"

#define MAP_PALLET_TOWN				 "PalletTown"
#define MAP_PALLET_TOWN_ASH_HOUSE_F0 "PalletTownAshHouseF0"
#define MAP_PALLET_TOWN_ASH_HOUSE_F1 "PalletTownAshHouseF1"
#define MAP_PALLET_TOWN_GARY_HOUSE	 "PalletTownGaryHouse"

DECLARE_MULTICAST_DELEGATE(DGameMapLoaded);
DECLARE_MULTICAST_DELEGATE(DGameMapUnloaded);

enum EMapState
{
	MS_None,
	MS_Loading,
	MS_Unloading,
	MS_Loaded,
};

class PMapManager
{
	TMap<std::string, JSON>		 mMapData;
	TMap<std::string, PGameMap*> mActiveMaps;
	EMapState					 mMapState = MS_None;

public:
	DGameMapLoaded	 GameMapLoaded;
	DGameMapUnloaded GameMapUnloaded;

	PGameMap* ConstructMap(const JSON& JsonData);
	PGameMap* GetMap(const std::string& Name);
	PGameMap* LoadMap(const std::string& Name, bool ForceReload);
	PGameMap* LoadMapFile(const std::string& FileName);

	bool UnloadMap(const std::string& Name);
	bool SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition,
				   EOrientation ExitDirection);

	PGameMap* GetMapUnderMouse();
	PGameMap* GetMapAtPosition(const FVector2& Position);
	void	  ClearMaps();

	EMapState GetState();
};

// Defined in World.h
static PMapManager* GetMapManager();