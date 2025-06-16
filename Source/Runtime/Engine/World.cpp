#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

#include <ranges>

void PWorld::Start()
{
	LogDebug("Starting world");

	LogDebug("Starting {} actors.", GetActors().size());
	for (const auto& Actor : GetActors())
	{
		Actor->Start();
	}
	LogDebug("Starting {} components.", GetComponents().size());
	for (const auto& Component : GetComponents())
	{
		Component->Start();
	}
}

void PWorld::Tick(float DeltaTime)
{
	for (const auto& Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
	}
	for (const auto& Component : GetComponents())
	{
		Component->Tick(DeltaTime);
	}
}

PChunk* PWorld::GetChunkAtPosition(const FVector2& Position) const
{
	for (const auto& Actor : GetActors())
	{
		if (const auto Chunk = dynamic_cast<PChunk*>(Actor))
		{
			auto Bounds = Chunk->GetWorldBounds();
			if (Bounds.Contains(Position))
			{
				return Chunk;
			}
		}
	}
	return nullptr;
}
