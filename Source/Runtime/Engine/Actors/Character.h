#pragma once

#include "Actor.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Mesh.h"

#include <memory>

class PCharacter : public PActor
{
protected:
	FRect mBounds;

public:
	PCharacter() = default;
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;
};
