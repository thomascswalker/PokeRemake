#pragma once

#include "../Object.h"
#include "Renderer/IDrawable.h"

class PActor : public PObject, public IDrawable
{
public:
	PActor() = default;
	~PActor() override = default;

	void Tick(float DeltaTime) override {}
	void Draw(IRenderer* Renderer) override = 0;
};
