#include "MapManager.h"

#include "Actors/PlayerCharacter.h"
#include "Core/Files.h"

#include "Serialization.h"
#include "World.h"

TMap<std::string, JSON>		 PMapManager::sMapData = {};
TMap<std::string, PGameMap*> PMapManager::sActiveMaps = {};

static EMapState gMapState = MS_None;
DGameMapLoaded	 PMapManager::GameMapLoaded = DGameMapLoaded();
DGameMapUnloaded PMapManager::GameMapUnloaded = DGameMapUnloaded();

PGameMap* PMapManager::ConstructMap(const JSON& JsonData)
{
	std::string MapName = JsonData["MapName"];
	if (sActiveMaps.Contains(MapName))
	{
		LogWarning("Map {} already exists.", MapName.c_str());
		return sActiveMaps[MapName];
	}
	// Create the map
	const auto GameMap = SpawnActor<PGameMap>(JsonData);
	if (!GameMap)
	{
		LogError("Failed to create map");
		return nullptr;
	}
	sActiveMaps[MapName] = GameMap;
	return GameMap;
}

PGameMap* PMapManager::GetMap(const std::string& Name)
{
	// If the map is available in the list of active maps, return it
	if (sActiveMaps.Contains(Name))
	{
		return sActiveMaps[Name];
	}

	return nullptr;
}

PGameMap* PMapManager::LoadMap(const std::string& Name, bool ForceReload)
{
	gMapState = MS_Loading;
	JSON JsonData;
	if (sMapData.Contains(Name) && !ForceReload)
	{
		JsonData = sMapData[Name];
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
			gMapState = MS_None;
			return nullptr;
		}

		JsonData = JSON::parse(Data.data());
		Expand(&JsonData);
		sMapData.Emplace(Name, JsonData);
	}
	const std::string MapName = JsonData["MapName"];
	sMapData[MapName] = JsonData;
	auto NewMap = ConstructMap(JsonData);
	if (!NewMap)
	{
		gMapState = MS_None;
		LogError("Failed to construct map.");
		return nullptr;
	}

	GameMapLoaded.Broadcast();
	gMapState = MS_Loaded;
	return NewMap;
}

PGameMap* PMapManager::LoadMapFile(const std::string& FileName)
{
	gMapState = MS_Loading;
	JSON JsonData;

	std::string Data;

	if (FileName.empty() || !Files::ReadFile(FileName, Data))
	{
		gMapState = MS_None;
		return nullptr;
	}

	JsonData = JSON::parse(Data.data());
	Expand(&JsonData);
	const std::string MapName = JsonData["MapName"];
	sMapData[MapName] = JsonData;

	auto NewMap = ConstructMap(JsonData);
	if (!NewMap)
	{
		gMapState = MS_None;
		LogError("Failed to construct map.");
		return nullptr;
	}
	GameMapLoaded.Broadcast();
	gMapState = MS_Loaded;
	return NewMap;
}

bool PMapManager::UnloadMap(const std::string& Name)
{
	gMapState = MS_Unloading;
	// Get the reference to the map
	auto GameMap = GetMap(Name);
	if (!GameMap)
	{
		gMapState = MS_None;
		LogError("Unable to find map {} in world.", Name.c_str());
		return false;
	}

	// Destroy the map actor
	GetWorld()->DestroyActor(GameMap);

	// Remove the map from the list of active maps
	sActiveMaps.Remove(Name);

	GameMapUnloaded.Broadcast();

	gMapState = MS_None;

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
	for (const auto GameMap : sActiveMaps | std::views::values)
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
	sActiveMaps.Clear();
}

EMapState PMapManager::GetState()
{
	return gMapState;
}
