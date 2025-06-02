#include "World.h"

#include "Engine/InputManager.h"

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