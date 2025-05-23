#include "Engine.h"
#include "InputManager.h"

#include <iostream>

static void PrintUp(uint32_t Value) { std::cout << "Up: " << Value << std::endl; }
static void PrintDown(uint32_t Value) { std::cout << "Down: " << Value << std::endl; }

Engine::Engine()
{
	bIsRunning = true;
	if (const auto InputManager = GetInputManager())
	{
		InputManager->KeyUp.AddStatic(&PrintUp);
		InputManager->KeyDown.AddStatic(&PrintDown);
	}
}

void Engine::Stop() { bIsRunning = false; }

void Engine::Tick(float DeltaTime) {}
