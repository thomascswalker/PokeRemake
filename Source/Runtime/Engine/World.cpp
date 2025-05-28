#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

void PWorld::Tick(float DeltaTime)
{
	for (const auto Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
	}
}