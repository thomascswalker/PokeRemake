#include "World.h"

#include "Engine/InputManager.h"

PWorld::PWorld() { mGrid = std::make_unique<PGrid>(25, 25, 16); }
