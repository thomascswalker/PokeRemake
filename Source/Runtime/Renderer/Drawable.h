#pragma once

enum EDrawPriority
{
	DP_FOREGROUND,
	DP_BACKGROUND,
};

class IDrawable
{
protected:
	EDrawPriority mRenderPriority = DP_BACKGROUND;

public:
	virtual ~IDrawable() = default;
	virtual bool  Draw(const PRenderer* Renderer) const = 0;
	virtual bool  DebugDraw(const PRenderer* Renderer) const { return true; }
	EDrawPriority GetPriority() const { return mRenderPriority; }
};