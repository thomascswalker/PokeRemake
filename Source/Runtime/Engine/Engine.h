#pragma once

#include "Game.h"

class PEngine
{
	/* State */
	bool bIsRunning = false;

	/* Actors/Objects */
	PGame* mGame;

public:
	PEngine();
	~PEngine() = default;
	void Stop();
	void Tick(float DeltaTime) const;
	bool IsRunning() const { return bIsRunning; }

	void   SetGame(PGame* Game) { mGame = Game; }
	PGame* GetGame() const { return mGame; }
};
