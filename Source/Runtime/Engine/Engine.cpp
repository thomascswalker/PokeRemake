#include "Engine.h"
#include "Core/Logging.h"
#include "InputManager.h"

static void PrintUp(uint32_t Value) { Warning("Up: {}", Value); }
static void PrintDown(uint32_t Value) { Error("Down: {}", Value); }

Engine::Engine()
{
	bIsRunning = true;
	if (const auto InputManager = GetInputManager())
	{
		InputManager->KeyUp.AddStatic(&PrintUp);
		InputManager->KeyDown.AddStatic(&PrintDown);
	}

	// Construct the world
	mWorld = std::make_unique<AWorld>();
}

void Engine::Stop() { bIsRunning = false; }

void Engine::Tick(float DeltaTime) {}
