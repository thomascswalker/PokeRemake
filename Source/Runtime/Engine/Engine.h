#pragma once

#include "World.h"

class PEngine
{
	/* State */
	bool bIsRunning = false;

	/* Actors/Objects */
	std::shared_ptr<PWorld> mWorld;

public:
	PEngine();
	~PEngine() = default;
	void	Stop();
	void	Tick(float DeltaTime) const;
	bool	IsRunning() const { return bIsRunning; }
	PWorld* GetWorld() const { return mWorld.get(); }
};

static PEngine* gEngine;
