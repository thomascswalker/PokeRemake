#pragma once

#include "Components/CameraComponent.h"
#include "Core/Settings.h"
#include "World.h"

class PGame : public PObject
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld>	   mWorld;
	std::shared_ptr<PSettings> mSettings;

	PCameraView* mActiveCameraView = nullptr;

public:
	PGame();
	~PGame() override {}

	void	Start() override;
	void	End() override {}
	void	Tick(float DeltaTime) override;
	PWorld* GetWorld() const
	{
		if (!mWorld.get())
		{
			return nullptr;
		}
		return mWorld.get();
	}

	void		 FindActiveCamera();
	PCameraView* GetCameraView() const { return mActiveCameraView; }
	PSettings*	 GetSettings() const { return mSettings.get(); }
};

DECLARE_STATIC_GLOBAL_GETTER(Game)
