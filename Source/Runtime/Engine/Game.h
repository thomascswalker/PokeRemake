#pragma once

#include "Components/CameraComponent.h"
#include "Core/Map.h"

#include "GameEvent.h"
#include "GameMode.h"
#include "Settings.h"
#include "World.h"

DECLARE_MULTICAST_DELEGATE(DGameStarted);
DECLARE_MULTICAST_DELEGATE(DGameEnded);

class PGame : public PObject, public IInputHandler
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld>	   mWorld;
	std::shared_ptr<SSettings> mSettings;
	PCameraView*			   mActiveCameraView = nullptr;

	/* Game State */

	bool										  mGameStarted = false;
	PGameMode*									  mGameMode = nullptr;
	TMap<std::string, std::shared_ptr<PGameMode>> mGameModes;

public:
	DGameStarted GameStarted;
	DGameEnded	 GameEnded;

	PGame();
	~PGame() override {}

	bool PreStart() override;
	bool Start() override;
	bool End() override;
	void Tick(float DeltaTime) override;

	PWorld* GetWorld() const
	{
		if (!mWorld.get())
		{
			return nullptr;
		}
		return mWorld.get();
	}

	void UpdateCameraView();

	PCameraView* GetCameraView() const
	{
		return mActiveCameraView;
	}

	void OnKeyUp(SInputEvent* Event) override;
	bool ProcessEvents(SInputEvent* Event) override;

	template <typename T>
	T* AddGameMode()
	{
		auto Mode = std::make_shared<T>();
		auto Name = Mode->GetName();
		mGameModes.Emplace(Name, Mode);
		if (mGameMode == nullptr)
		{
			mGameMode = Mode.get();
		}
		return dynamic_cast<T*>(mGameModes[Name].get());
	}

	PGameMode* GetGameMode(const std::string& Name)
	{
		return mGameModes[Name].get();
	}

	PGameMode* GetCurrentGameMode() const
	{
		return mGameMode;
	}

	bool SetCurrentGameMode(const std::string& Name);
	bool LoadCurrentGameMode();
	bool SetAndLoadCurrentGameMode(const std::string& Name);

	virtual void OnGameModeLoaded(PGameMode* GameMode);
	virtual void OnGameModeUnloaded(PGameMode* GameMode);

	virtual bool HandleGameEvent(SGameEvent& Event)
	{
		return true;
	}
};
