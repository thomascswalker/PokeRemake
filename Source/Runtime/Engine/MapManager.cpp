#include "MapManager.h"

#include "Actors/PlayerCharacter.h"
#include "Components/CollisionComponent.h"
#include "Core/Files.h"

#include "Serialization.h"
#include "World.h"

PMapManager* GMapManager = nullptr;

PGameMap* PMapManager::ConstructMap(const JSON& JsonData)
{
	std::string MapName = JsonData["MapName"];
	if (mActiveMaps.Contains(MapName))
	{
		LogWarning("Map {} already exists.", MapName.c_str());
		return mActiveMaps[MapName];
	}
	// Create the map first
	const auto GameMap = SpawnActor<PGameMap>(JsonData);
	if (!GameMap)
	{
		LogError("Failed to create map");
		return nullptr;
	}
	mActiveMaps[MapName] = GameMap;

	// Deserialize after it's been added to the map list
	for (auto Child : JsonData["Children"])
	{
		if (auto ChildActor = GSerializer->DeserializeActor(Child))
		{
			GameMap->AddChild(ChildActor);
		}
	}
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
	SetState(MS_Loading);
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
			SetState(MS_Unloaded);
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
		SetState(MS_Unloaded);
		LogError("Failed to construct map.");
		return nullptr;
	}

	SetState(MS_Loaded);
	return NewMap;
}

PGameMap* PMapManager::LoadMapFromFile(const std::string& FileName)
{
	SetState(MS_Loading);
	JSON JsonData;

	std::string Data;

	if (FileName.empty() || !Files::ReadFile(FileName, Data))
	{
		SetState(MS_Unloaded);
		return nullptr;
	}

	JsonData = JSON::parse(Data.data());
	Expand(&JsonData);
	const std::string MapName = JsonData["MapName"];
	mMapData[MapName] = JsonData;

	auto NewMap = ConstructMap(JsonData);
	if (!NewMap)
	{
		SetState(MS_Unloaded);
		LogError("Failed to construct map.");
		return nullptr;
	}
	SetState(MS_Loaded);
	return NewMap;
}

bool PMapManager::UnloadMap(const std::string& Name)
{
	// Get the reference to the map
	auto GameMap = GetMap(Name);
	if (!GameMap)
	{
		SetState(MS_Unloaded);
		LogError("Unable to find map {} in world.", Name.c_str());
		return false;
	}

	// Destroy the map actor
	GWorld->DestroyActor(GameMap);

	// Remove the map from the list of active maps
	mActiveMaps.Remove(Name);

	SetState(MS_Unloaded);

	return true;
}

void PMapManager::UnloadSwitchMap()
{
	// Clear the timer which triggered this deferred map unloading
	GTimerManager->ClearTimer(mUnloadHandle);

	// Actually unload the map
	UnloadMap(mSwitchMap.OldMap);

	// Load in the new map
	PGameMap* GameMap = LoadMap(mSwitchMap.NewMap, false);
	if (!GameMap)
	{
		LogError("Failed to load map: {}", mSwitchMap.NewMap.c_str());
		return;
	}
	if (auto Player = GWorld->GetPlayerCharacter())
	{
		Player->GetMovementComponent()->SetMovementDirection(mSwitchMap.ExitDirection);
		Player->GetMovementComponent()->SnapToPosition(mSwitchMap.NewPosition, GameMap);
		Player->GetComponent<PCollisionComponent>()->SetCollideable(true);
	}
	else
	{
		LogError("Failed to get Player Character.");
	}
}

bool PMapManager::SwitchMap(const std::string& OldMap, const std::string& NewMap, const FVector2& NewPosition,
							EOrientation ExitDirection, float Delay)
{
	mSwitchMap = {
		OldMap,
		NewMap,
		NewPosition,
		ExitDirection,
		Delay,
	};

	SetState(MS_Unloading);

	// For the duration of switching the map, make the player uncollideable.
	GWorld->GetPlayerCharacter()->GetComponent<PCollisionComponent>()->SetCollideable(false);

	// Defer unloading the map for 1 second to allow for the transition animation to play.
	GTimerManager->Delay(mUnloadHandle, Delay, this, &PMapManager::UnloadSwitchMap);

	return true;
}

PGameMap* PMapManager::GetMapUnderMouse()
{
	auto Actors = GRenderer->GetActorsUnderMouse();
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

EMapState PMapManager::GetState() const
{
	return mMapState;
}

void PMapManager::SetState(EMapState NewState)
{
	mMapState = NewState;
	GameMapStateChanged.Broadcast(mMapState);
}
