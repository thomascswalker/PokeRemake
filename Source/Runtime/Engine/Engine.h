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
	void Start();
	void Stop();
	void Tick(float DeltaTime) const;
	void PostTick() const;

	bool IsRunning() const
	{
		return mIsRunning;
	}

	template <typename GameType>
	void Initialize()
	{
		mGame = std::make_unique<GameType>();
		mGame->GameStarted.AddRaw(this, &PEngine::Start);
		mGame->GameEnded.AddRaw(this, &PEngine::Stop);
	}

	PGame* GetGame() const
	{
		return mGame.get();
	}
};

DECLARE_STATIC_GLOBAL_GETTER(Engine)
