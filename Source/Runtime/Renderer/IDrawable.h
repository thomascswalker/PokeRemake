#pragma once

#include "Renderer/Generic/IRenderer.h"

class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void Draw(IRenderer* Renderer) = 0;
};