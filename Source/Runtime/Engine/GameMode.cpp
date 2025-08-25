#include "GameMode.h"

#include "Game.h"

void PGameMode::Start()
{
    if (!Load())
    {
        LogError("Failed to load game mode.");
    }
}
