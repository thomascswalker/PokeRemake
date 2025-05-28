#pragma once

#include "Core/Vector.h"
#include "Engine/Object.h"
#include "Renderer/IDrawable.h"

class PActor : public PObject, public IDrawable
{
protected:
	FVector3 mPosition;

public:
	PActor() = default;
	~PActor() override = default;

	void Start() override {}
	void End() override {}

	void Tick(float DeltaTime) override {}
	void Draw(IRenderer* Renderer) override = 0;

	FVector3 GetPosition() const { return mPosition; }
	void	 SetPosition(const FVector3& Position) { mPosition = Position; }
	void	 AddPosition(const FVector3& Position) { mPosition += Position; }
};
