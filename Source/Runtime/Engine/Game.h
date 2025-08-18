#pragma once

#include "Components/CameraComponent.h"
#include "Settings.h"
#include "World.h"

class PGame : public PObject, public IInputHandler
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld> mWorld;
	std::shared_ptr<PSettings> mSettings;

	PCameraView* mActiveCameraView = nullptr;

public:
	PGame();
	~PGame() override {}

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

	void FindActiveCamera();

	PCameraView* GetCameraView() const
	{
		return mActiveCameraView;
	}

	PSettings* GetSettings() const
	{
		return mSettings.get();
	}

	void OnKeyUp(SInputEvent* Event) override;
};

DECLARE_STATIC_GLOBAL_GETTER(Game)
