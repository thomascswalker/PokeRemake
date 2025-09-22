#include "MapManager.h"

#include "Actors/PlayerCharacter.h"
#include "Core/Files.h"

#include "Serialization.h"
#include "World.h"

PGameMap* PMapManager::ConstructMap(const JSON& JsonData)
{
	std::string MapName = JsonData["MapName"];
	if (mActiveMaps.Contains(MapName))
	{
		LogWarning("Map {} already exists.", MapName.c_str());
		return mActiveMaps[MapName];
	}
	// Create the map
	const auto GameMap = SpawnActor<PGameMap>(JsonData);
	if (!GameMap)
	{
		LogError("Failed to create map");
		return nullptr;
	}
	mActiveMaps[MapName] = GameMap;
	return GameMap;
}

PGameMap* PMapManager::GetMap(const std::string& Name)
{
	// If the map is available in the list of active maps, return it
	if (mActiveMaps.Contains(Name))
	{
		return mActiveMaps[Name];
	}

	return nullptr;
}

PGameMap* PMapManager::LoadMap(const std::string& Name, bool ForceReload)
{
	mMapState = MS_Loading;
	JSON JsonData;
	if (mMapData.Contains(Name) && !ForceReload)
	{
		JsonData = mMapData[Name];
	}
	else
	{
		std::string Data;
		std::string FileName;
		if (!Name.ends_with(".JSON"))
		{
			FileName = Files::FindFile(Name + ".JSON");
		}
		else
		{
			FileName = Name;
		}

		if (FileName.empty() || !Files::ReadFile(FileName, Data))
		{
			mMapState = MS_None;
			return nullptr;
		}

		JsonData = JSON::parse(Data.data());
		Expand(&JsonData);
		mMapData.Emplace(Name, JsonData);
	}
	const std::string MapName = JsonData["MapName"];
	mMapData[MapName] = JsonData;
	auto NewMap = ConstructMap(JsonData);
	if (!NewMap)
	{
		mMapState = MS_None;
		LogError("Failed to construct map.");
		return nullptr;
	}

	GameMapLoaded.Broadcast();
	mMapState = MS_Loaded;
	return NewMap;
}

PGameMap* PMapManager::LoadMapFile(const std::string& FileName)
{
	mMapState = MS_Loading;
	JSON JsonData;

	std::string Data;

	if (FileName.empty() || !Files::ReadFile(FileName, Data))
	{
		mMapState = MS_None;
		return nullptr;
	}

	JsonData = JSON::parse(Data.data());
	Expand(&JsonData);
	const std::string MapName = JsonData["MapName"];
	mMapData[MapName] = JsonData;

	auto NewMap = ConstructMap(JsonData);
	if (!NewMap)
	{
		mMapState = MS_None;
		LogError("Failed to construct map.");
		return nullptr;
	}
	GameMapLoaded.Broadcast();
	mMapState = MS_Loaded;
	return NewMap;
}

bool PMapManager::UnloadMap(const std::string& Name)
{
	mMapState = MS_Unloading;
	// Get the reference to the map
	auto GameMap = GetMap(Name);
	if (!GameMap)
	{
		mMapState = MS_None;
		LogError("Unable to find map {} in world.", Name.c_str());
		return false;
	}

	// Destroy the map actor
	GetWorld()->DestroyActor(GameMap);

	// Remove the map from the list of active maps
	mActiveMaps.Remove(Name);

	GameMapUnloaded.Broadcast();

	mMapState = MS_None;

	return true;
}

bool PMapManager::SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition,
							EOrientation ExitDirection)
{
	UnloadMap(OldMap);
	PGameMap* GameMap = LoadMap(NewMap, false);
	if (!GameMap)
	{
		LogError("Failed to load map: {}", NewMap.c_str());
		return false;
	}
	if (auto Player = GetWorld()->GetPlayerCharacter())
	{
		Player->GetMovementComponent()->SetMovementDirection(ExitDirection);
		Player->GetMovementComponent()->SnapToPosition(NewPosition, GameMap);
	}
	else
	{
		LogError("Failed to get Player Character.");
		return false;
	}

	return true;
}

PGameMap* PMapManager::GetMapUnderMouse()
{
	auto Actors = GetRenderer()->GetActorsUnderMouse();
	for (auto Actor : Actors)
	{
		if (auto GameMap = dynamic_cast<PGameMap*>(Actor))
		{
			return GameMap;
		}
	}
	return nullptr;
}

PGameMap* PMapManager::GetMapAtPosition(const FVector2& Position)
{
	for (const auto GameMap : mActiveMaps | std::views::values)
	{
		auto Bounds = GameMap->GetWorldBounds();
		Bounds.W *= 2.0f;
		Bounds.H *= 2.0f;
		if (Bounds.Contains(Position))
		{
			return GameMap;
		}
	}
	return nullptr;
}

void PMapManager::ClearMaps()
{
	mActiveMaps.Clear();
}

EMapState PMapManager::GetState()
{
	return mMapState;
}
