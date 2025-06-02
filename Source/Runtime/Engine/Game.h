#pragma once

#include "Components/CameraComponent.h"
#include "World.h"

class PGame : public PObject
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld> mWorld;

	PCameraView* mActiveCameraView = nullptr;

public:
	PGame();
	~PGame() override {}

	void	Start() override;
	void	End() override {}
	void	Tick(float DeltaTime) override;
	PWorld* GetWorld() const { return mWorld.get(); }

	void		 FindActiveCamera();
	PCameraView* GetActiveCameraView() const { return mActiveCameraView; }
};

PGame* GetGame();