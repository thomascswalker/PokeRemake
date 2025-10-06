#pragma once

#include "Game.h"
#include "GameInstance.h"

class PEngine : public PObject
{
	/* State */
	bool mIsRunning = false;

	std::shared_ptr<PGame> mGame = nullptr;
	PGameInstance		   mGameInstance;
	PSerializer			   mSerializer;
	PTextureManager		   mTextureManager;

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

		// Associate global managers
		GGameInstance = &mGameInstance;
		GTextureManager = &mTextureManager;

		// Construct the game
		mGame = std::make_shared<TGameType>();

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

	template <typename T>
	T* GetGameAs() const
	{
		return dynamic_cast<T*>(mGame.get());
	}

	PGameInstance* GetGameInstance()
	{
		return &mGameInstance;
	}

	SGameState* GetCurrentGameState()
	{
		return mGame->GetCurrentGameMode()->GetState();
	}

	SGameState* GetGameState(const std::string& Name)
	{
		return mGame->GetGameMode(Name)->GetState();
	}
};

extern PEngine* GEngine;
