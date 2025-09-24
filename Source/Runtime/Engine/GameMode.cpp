#include "GameMode.h"

#include "Game.h"

void PGameMode::PostLoad()
{
	GetGame()->UpdateCameraView();
}
void PGameMode::PostUnload()
{
}