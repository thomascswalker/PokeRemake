#include "Game.h"

#include "Core/Logging.h"

#include <memory>

PGame::PGame() { mWorld = std::make_shared<PWorld>(); }

void PGame::Start()
{
	for (const auto& Actor : mWorld->GetActors())
	{
		Actor->Start();
	}
}
void PGame::Tick(float DeltaTime) { mWorld->Tick(DeltaTime); }
