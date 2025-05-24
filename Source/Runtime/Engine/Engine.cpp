#include "Engine.h"
#include "Core/Logging.h"
#include "InputManager.h"

static void PrintUp(uint32_t Value) { Warning("ScanCode: {:03x}", Value); }

PEngine::PEngine()
{
	bIsRunning = true;
	if (const auto InputManager = GetInputManager())
	{
		InputManager->KeyUp.AddStatic(&PrintUp);
	}

	// Construct the world
	mWorld = std::make_unique<PWorld>();
}

void PEngine::Stop() { bIsRunning = false; }

void PEngine::Tick(float DeltaTime) { mWorld->GetGrid()->Tick(DeltaTime); }
