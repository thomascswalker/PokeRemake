#include "Game.h"

#include <memory>

PGame::PGame() { mWorld = std::make_shared<PWorld>(); }

void PGame::Tick(float DeltaTime) { mWorld->Tick(DeltaTime); }
