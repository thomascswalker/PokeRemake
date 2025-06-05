#pragma once

#include "Renderer/Renderer.h"

enum EDrawPriority
{
	DP_FOREGROUND,
	DP_BACKGROUND,
};

class IDrawable
{
protected:
	EDrawPriority mPriority = DP_BACKGROUND;

public:
	virtual ~IDrawable() = default;
	virtual void  Draw(const PRenderer* Renderer) const = 0;
	EDrawPriority GetPriority() const { return mPriority; }
};