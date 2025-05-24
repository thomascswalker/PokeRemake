#pragma once

#include "Actors/World.h"

class Engine
{
	bool					bIsRunning = false;
	std::unique_ptr<AWorld> mWorld;

public:
	Engine();
	void	Stop();
	void	Tick(float DeltaTime);
	bool	IsRunning() const { return bIsRunning; }
	AWorld* GetWorld() const { return mWorld.get(); }
};
