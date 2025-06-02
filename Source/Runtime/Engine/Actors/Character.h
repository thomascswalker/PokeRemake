#pragma once

#include "Actor.h"

class PCharacter : public PActor
{
protected:
	FRect	 mBounds;
	FVector2 mVelocity;

public:
	PCharacter();
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;
};
