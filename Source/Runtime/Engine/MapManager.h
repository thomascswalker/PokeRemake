#pragma once

#include "Actors/GameMap.h"
#include "Core/Map.h"

#define MAP_PALLET_TOWN				 "PalletTown"
#define MAP_PALLET_TOWN_ASH_HOUSE_F0 "PalletTownAshHouseF0"
#define MAP_PALLET_TOWN_ASH_HOUSE_F1 "PalletTownAshHouseF1"
#define MAP_PALLET_TOWN_GARY_HOUSE	 "PalletTownGaryHouse"

enum EMapState
{
	MS_Loading,
	MS_Loaded,
	MS_Unloading,
	MS_Unloaded,
};

DECLARE_MULTICAST_DELEGATE(DGameMapStateChange, EMapState);

struct SSwitchMapParams
{
	std::string	 OldMap;
	std::string	 NewMap;
	FVector2	 NewPosition;
	EOrientation ExitDirection;
	float		 Delay;
};

class PMapManager
{
	TMap<std::string, JSON>		 mMapData;
	TMap<std::string, PGameMap*> mActiveMaps;
	EMapState					 mMapState = MS_Unloaded;
	SSwitchMapParams			 mSwitchMap = {};

public:
	DGameMapStateChange GameMapStateChanged;

	STimerHandle mLoadHandle;
	STimerHandle mUnloadHandle;

	PGameMap* ConstructMap(const JSON& JsonData);
	PGameMap* GetMap(const std::string& Name);
	PGameMap* LoadMap(const std::string& Name, bool ForceReload);
	PGameMap* LoadMapFromFile(const std::string& FileName);

	bool UnloadMap(const std::string& Name);
	void UnloadSwitchMap();
	bool SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition,
				   EOrientation ExitDirection, float Delay);

	PGameMap* GetMapUnderMouse();
	PGameMap* GetMapAtPosition(const FVector2& Position);
	void	  ClearMaps();

	EMapState GetState() const;
	void	  SetState(EMapState NewState);
};

// Defined in World.h
extern PMapManager* GMapManager;