#include "Engine.h"

PEngine::PEngine()
	: mGame(nullptr) {}

void PEngine::Start()
{
	LogDebug("Starting engine.");
	mIsRunning = true;
}

void PEngine::Stop()
{
	LogDebug("Stopping engine.");
	mIsRunning = false;
}

void PEngine::Tick(float DeltaTime) const
{
	if (mGame)
	{
		mGame->Tick(DeltaTime);
	}
}

void PEngine::PostTick() const
{
	GetGame()->PostTick();
}
