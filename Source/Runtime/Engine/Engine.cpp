#include "Engine.h"
#include "Core/Logging.h"
#include "InputManager.h"

PEngine::PEngine()
{
	bIsRunning = true;

	// Construct the world
	mWorld = std::make_shared<PWorld>();
}

void PEngine::Stop() { bIsRunning = false; }

void PEngine::Tick(float DeltaTime) const
{
	for (PActor* Actor : mWorld->GetActors())
	{
		Actor->Tick(DeltaTime);
	}
}
