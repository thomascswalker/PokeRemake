#pragma once

#include "GameMode.h"

#include "Components/CameraComponent.h"
#include "Settings.h"
#include "World.h"

#include "Core/Map.h"

class PGame : public PObject, public IInputHandler
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld> mWorld;
	std::shared_ptr<PSettings> mSettings;
	PCameraView* mActiveCameraView = nullptr;

	/* Game State */

	PGameMode* mGameMode = nullptr;
	TMap<std::string, std::shared_ptr<PGameMode>> mGameModes;

public:
	~PGame() override {}

	bool PreStart() override;
	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void PostTick() override;

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

	PSettings* GetSettings() const
	{
		return mSettings.get();
	}

	void OnKeyUp(SInputEvent* Event) override;
	bool ProcessEvents(SInputEvent* Event) override;

	template <typename T>
	T* AddGameMode()
	{
		auto Mode = std::make_shared<T>();
		auto Name = Mode->GetName();
		mGameModes.Add(Name, Mode);
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

	virtual void OnGameModeLoaded(PGameMode* GameMode);
	virtual void OnGameModeUnloaded(PGameMode* GameMode);
};

DECLARE_STATIC_GLOBAL_GETTER(Game)
