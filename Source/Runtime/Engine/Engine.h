#pragma once

#include "Actors/World.h"

class PEngine
{
	bool					bIsRunning = false;
	std::unique_ptr<PWorld> mWorld;

public:
	PEngine();
	void	Stop();
	void	Tick(float DeltaTime);
	bool	IsRunning() const { return bIsRunning; }
	PWorld* GetWorld() const { return mWorld.get(); }
};
