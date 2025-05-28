#pragma once

#include "Renderer/Renderer.h"

class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void Draw(PRenderer* Renderer) = 0;
};