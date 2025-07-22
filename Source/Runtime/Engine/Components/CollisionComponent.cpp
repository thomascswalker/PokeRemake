#include "CollisionComponent.h"

#include "Engine/World.h"

#include <thread>

void PCollisionComponent::Tick(float DeltaTime)
{
	auto W = GetWorld();
	if (!W)
	{
		LogError("World is invalid.");
		return;
	}
	if (!mOwner)
	{
		LogError("Owner is invalid.");
		return;
	}
	auto OwnerPosition = mOwner->GetPosition();
	auto Actors = W->GetActorsAtPosition(OwnerPosition);

	// Loop through all actors at the current position and if they're
	// not currently in our overlapping actors array, add them and broadcast
	// that we've begun overlapping.
	for (auto Actor : Actors)
	{
		// Ignore the owner
		if (Actor == mOwner)
		{
			continue;
		}
		if (!Containers::Contains(mOverlappingActors, Actor))
		{
			mOverlappingActors.push_back(Actor);
			OverlapBegin.Broadcast(Actor);
		}
	}
	// Loop through all actors in our overlapping actors array and if they're
	// not currently overlapping, remove them from the array and broadcast
	// that we've ended overlapping.
	for (auto Actor : mOverlappingActors)
	{
		// Ignore the owner
		if (Actor == mOwner)
		{
			continue;
		}
		if (!Containers::Contains(Actors, Actor))
		{
			Containers::Remove(mOverlappingActors, Actor);
			OverlapEnd.Broadcast(Actor);
		}
	}
}