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
	void PostTick() const;

	bool IsRunning() const
	{
		return mIsRunning;
	}

	/**
	 * @brief Initialize and start the engine
	 * - Sets the engine to running.
	 * - Constructs the GameType.
	 * - Starts the game.
	 * - Constructs the HUD.
	 *
	 * @tparam GameType The game type to construct.
	 * @return
	 */
	template <class GameType>
	bool Start()
	{
		LogDebug("Starting engine.");
		mIsRunning = true;

		// Construct the game
		// At this point:
		// 1. The world exists (but has not started).
		// 2. Settings exist
		mGame = std::make_unique<GameType>();

		// Bind the game ending to stopping the engine
		mGame->GameEnded.AddRaw(this, &PEngine::Stop);

		// Attempt to start the game. If this fails, exit the engine and therefore
		// the application.
		// 1. Load all textures.
		// 2. Load all tilesets
		// 3. Load all stylesheets
		// 4. Construct the HUD
		if (!mGame->PreStart())
		{
			LogError("Failed to pre-start game");
			mIsRunning = false;
			return false;
		}

		// The world is started.
		if (!mGame->Start())
		{
			LogError("Failed to start game.");
			mIsRunning = false;
			return false;
		}

		return true;
	}

	PGame* GetGame() const
	{
		return mGame.get();
	}
};

DECLARE_STATIC_GLOBAL_GETTER(Engine)
