#pragma once

#include "Game.h"

class PEngine
{
	/* State */
	bool bIsRunning = false;

	/* Actors/Objects */
	std::unique_ptr<PGame> mGame;

public:
	PEngine();
	~PEngine() = default;
	void Stop();
	void Tick(float DeltaTime) const;
	bool IsRunning() const { return bIsRunning; }

	template <typename GameType>
	void StartGame()
	{
		mGame = std::make_unique<GameType>();
	}
	PGame* GetGame() const { return mGame.get(); }
};

DECLARE_STATIC_GLOBAL_GETTER(Engine)
