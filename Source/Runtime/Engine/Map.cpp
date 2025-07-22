#include "Map.h"

#include "Actors/PlayerCharacter.h"
#include "Core/Files.h"
#include "Serializer.h"
#include "World.h"

PChunk* PMapManager::GetMapInWorld(const std::string& Name)
{
	for (auto Actor : GetWorld()->GetActors())
	{
		if (auto Chunk = dynamic_cast<PChunk*>(Actor))
		{
			if (Chunk->GetMapName() == Name)
			{
				return Chunk;
			}
		}
	}
	return nullptr;
}

PChunk* PMapManager::LoadMap(const std::string& Name)
{
	LogDebug("Loading map: {}", Name.c_str());
	std::string Data;
	std::string FileName = Files::FindFile(Name + ".JSON");

	if (FileName.empty() || !Files::ReadFile(FileName, Data))
	{
		return nullptr;
	}

	const json JsonData = json::parse(Data.data());

	if (JsonData.at("Class") != "PChunk")
	{
		LogError("Invalid class at root level of map file. Must be 'PChunk'.");
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

bool PMapManager::SwitchMap(const std::string& OldMap, const std::string& NewMap, const IVector2& NewPosition)
{
	UnloadMap(OldMap);
	if (auto Chunk = LoadMap(NewMap))
	{
		LogDebug("Switched to map: {}", NewMap.c_str());

		if (auto Player = GetWorld()->GetPlayerCharacter())
		{
			Player->GetMovementComponent()->SetCurrentChunk(Chunk);
			Player->GetMovementComponent()->SnapToTile(NewPosition);
		}
	}
	return true;
}