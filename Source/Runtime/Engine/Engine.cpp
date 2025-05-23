#include "Engine.h"
#include "InputManager.h"

#include <iostream>

static void Print(uint32_t Value) { std::cout << "Value: " << Value << std::endl; }

Engine::Engine()
{
	bIsRunning = true;
	if (const auto InputManager = GetInputManager())
	{
		InputManager->KeyUp.AddStatic(&Print);
	}
}

void Engine::Stop() { bIsRunning = false; }

void Engine::Tick(float DeltaTime) {}
