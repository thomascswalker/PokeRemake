#pragma once

#include "Object.h"
#include "Renderer/IDrawable.h"

class AActor : public Object, public IDrawable
{
public:
	AActor() = default;
	~AActor() override = default;

	void Draw(IRenderer* Renderer) override = 0;
};
