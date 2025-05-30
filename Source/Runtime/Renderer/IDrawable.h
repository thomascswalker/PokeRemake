#pragma once

#include "Renderer/Renderer.h"

class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void Draw(const PRenderer* Renderer) const = 0;
};