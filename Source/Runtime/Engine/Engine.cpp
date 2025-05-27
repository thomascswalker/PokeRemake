#include "Engine.h"
#include "Core/Logging.h"

PEngine::PEngine() { bIsRunning = true; }

void PEngine::Stop() { bIsRunning = false; }

void PEngine::Tick(float DeltaTime) const
{
	if (mGame)
	{
		mGame->Tick(DeltaTime);
	}
}
