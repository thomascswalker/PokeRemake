#pragma once

#include "Game.h"

class PEngine
{
	/* State */
	bool mIsRunning = false;

	/* Actors/Objects */
	std::unique_ptr<PGame> mGame;

public:
	PEngine();
	~PEngine() = default;
	void Stop();
	void Tick(float DeltaTime) const;
	bool IsRunning() const { return mIsRunning; }

	template <typename GameType>
	void Start()
	{
		mGame = std::make_unique<GameType>();
	}

	PGame* GetGame() const { return mGame.get(); }
};

DECLARE_STATIC_GLOBAL_GETTER(Engine)
