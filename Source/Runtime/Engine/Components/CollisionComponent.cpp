#include "CollisionComponent.h"
#include "Engine/World.h"

void PCollisionComponent::ProcessCollision()
{
	auto W = GWorld;
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
	auto OwnerPosition = mOwner->GetPosition2D();
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
		// Add to overlapping actors
		if (!Containers::Contains(mOverlappingActors, Actor))
		{
			Containers::Add(mOverlappingActors, Actor);
			OverlapBegin.Broadcast(Actor);
		}
		// Remove from overlapping actors
		else if (!Containers::Contains(Actors, Actor))
		{
			Containers::Remove(mOverlappingActors, Actor);
			OverlapEnd.Broadcast(Actor);
		}
	}
}