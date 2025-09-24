#include "Engine.h"

PEngine* GEngine = nullptr;

bool PEngine::PreStart()
{
	// Attempt to prestart the game. If this fails, exit the engine and therefore
	// the application.
	if (!mGame->PreStart())
	{
		LogError("Failed to pre-start game");
		mIsRunning = false;
		return false;
	}
	return true;
}

bool PEngine::Start()
{
	// Attempt to start the game. If this fails, exit the engine and therefore
	// the application.
	if (!mGame->Start())
	{
		LogError("Failed to start game.");
		mIsRunning = false;
		return false;
	}
	return true;
}

void PEngine::Stop()
{
	LogDebug("Stopping engine.");
	mIsRunning = false;
}

void PEngine::Tick(float DeltaTime)
{
	// Tick the game instance. This will:
	// 1. Remove all objects queued for destruction.
	mGameInstance.Tick(DeltaTime);

	// Tick the game itself
	mGame->Tick(DeltaTime);
}

void PEngine::PostTick() const
{
	GetGame()->PostTick();
}
