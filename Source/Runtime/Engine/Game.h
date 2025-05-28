#pragma once

#include "World.h"

class PGame : public PObject
{
protected:
	/* Actors/Objects */
	std::shared_ptr<PWorld> mWorld;

public:
	PGame();
	~PGame() override {}

	void	Start() override;
	void	End() override {}
	void	Tick(float DeltaTime) override;
	PWorld* GetWorld() const { return mWorld.get(); }
};
