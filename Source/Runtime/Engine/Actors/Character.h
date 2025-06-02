#pragma once

#include "Actor.h"
#include "Engine/Components/CameraComponent.h"

#include <memory>

class PCharacter : public PActor
{
protected:
	std::vector<FVector2> mVertices;
	std::vector<int32_t>  mIndices;

public:
	PCharacter() = default;
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;
};
