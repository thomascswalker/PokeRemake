#pragma once

#include "Game.h"
#include "GameInstance.h"

class PEngine : public PObject
{
	/* State */
	bool mIsRunning = false;

	std::unique_ptr<PGame> mGame = nullptr;
	PGameInstance		   mGameInstance;

public:
	/**
	 * @brief Initialize and start the engine
	 * - Sets the engine to running.
	 * - Constructs the GameType.
	 * - Starts the game.
	 * - Constructs the HUD.
	 *
	 * @tparam TGameType The game type to construct.
	 * @return
	 */
	template <class TGameType>
	bool Init()
	{
		LogDebug("Starting engine.");
		mIsRunning = true;

		// Construct the game
		mGame = std::make_unique<TGameType>();

		// Construct game instance
		GGameInstance = &mGameInstance;

		// Bind the game ending to stopping the engine
		mGame->GameEnded.AddRaw(this, &PEngine::Stop);

		return true;
	}

	bool PreStart() override;
	bool Start() override;
	void Stop();
	void Tick(float DeltaTime) override;
	void PostTick() const;

	bool IsRunning() const
	{
		return mIsRunning;
	}

	PGame* GetGame() const
	{
		return mGame.get();
	}

	PGameInstance* GetGameInstance()
	{
		return &mGameInstance;
	}
};

extern PEngine* GEngine;
