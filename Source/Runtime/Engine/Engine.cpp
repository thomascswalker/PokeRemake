#include <format>
#include <iostream>

#include "Engine.h"

Engine::Engine() { bIsRunning = true; }

void Engine::Stop() { bIsRunning = false; }

void Engine::Tick(float DeltaTime) {}
