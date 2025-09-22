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
	 * @tparam HUDType The HUD type to construct.
	 * @return
	 */
	template <class GameType, class HUDType>
	bool Start()
	{
		LogDebug("Starting engine.");
		mIsRunning = true;

		// Construct the game
		mGame = std::make_unique<GameType>();

		// Bind the game ending to stopping the engine
		mGame->GameEnded.AddRaw(this, &PEngine::Stop);

		// Attempt to start the game. If this fails, exit the engine and therefore
		// the application.
		if (!mGame->PreStart())
		{
			LogError("Failed to pre-start game");
			mIsRunning = false;
			return false;
		}
		// Inbetween PreStart and Start, construct the HUD.
		mGame->GetWorld()->CreateHUD<HUDType>();

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
