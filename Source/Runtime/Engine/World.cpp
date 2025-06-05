#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

void PWorld::Start()
{
	LogDebug("Starting world");
	mGrid = this->ConstructActor<PGrid>();
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