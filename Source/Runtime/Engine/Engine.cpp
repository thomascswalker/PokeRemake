#include "Engine.h"

#include "Actors/Portal.h"

PEngine::PEngine()
	: mIsRunning(true), mGame(nullptr) {}

void PEngine::Stop()
{
	mIsRunning = false;
}

void PEngine::Tick(float DeltaTime) const
{
	if (mGame)
	{
		mGame->Tick(DeltaTime);
	}
}
