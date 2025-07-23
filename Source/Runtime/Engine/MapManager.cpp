#include "MapManager.h"

#include "Actors/PlayerCharacter.h"
#include "Core/Files.h"
#include "Serializer.h"
#include "World.h"

std::map<std::string, JSON> PMapManager::sMapData = {};

PMap* PMapManager::GetMapInWorld(const std::string& Name)
{
	LogDebug("Finding map: {}", Name.c_str());
	for (auto Map : GetWorld()->GetActorsOfType<PMap>())
	{
		LogDebug("Comparing map: {}", Map->GetMapName().c_str());
		if (Map->GetMapName() == Name)
		{
			return Map;
		}
	}
	return nullptr;
}

PMap* PMapManager::LoadMap(const std::string& Name)
{
	JSON JsonData;
	if (Containers::Contains(sMapData, Name))
	{
		LogDebug("Loading map from memory: {}", Name.c_str());
		JsonData = sMapData[Name];
	}
	else
	{
		LogDebug("Loading map from file: {}", Name.c_str());
		std::string Data;
		std::string FileName = Files::FindFile(Name + ".JSON");

		if (FileName.empty() || !Files::ReadFile(FileName, Data))
		{
			return nullptr;
		}

		JsonData = JSON::parse(Data.data());
		sMapData[Name] = JsonData;
	}

	if (JsonData.at("Class") != "PMap")
	{
		LogError("Invalid class at root level of map file. Must be 'PMap'.");
		return nullptr;
	}
	PSerializer::Deserialize(JsonData);

	return GetMapInWorld(Name);
}

bool PMapManager::UnloadMap(const std::string& Name)
{
	auto Map = GetMapInWorld(Name);
	if (!Map)
	{
		LogError("Unable to find map {} in world.", Name.c_str());
		return false;
	}

	LogDebug("Destroying map: {}", Name.c_str());
	GetWorld()->DestroyActor(Map);

	return true;
}

bool PMapManager::SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition)
{
	UnloadMap(OldMap);
	PMap* Map = LoadMap(NewMap);
	if (!Map)
	{
		LogError("Failed to load map: {}", NewMap.c_str());
		return false;
	}
	LogDebug("Switched to map: {}", NewMap.c_str());
	if (auto Player = GetWorld()->GetPlayerCharacter())
	{
		Player->GetMovementComponent()->SnapToPosition(NewPosition, Map);
	}
	else
	{
		LogError("Failed to get Player Character.");
		return false;
	}

	return true;
}