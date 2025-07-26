#pragma once

enum EDrawPriority
{
	DP_BACKGROUND = 0,
	DP_FOREGROUND = 1,
};

inline std::array gDrawPriorities = {DP_BACKGROUND, DP_FOREGROUND};

class IDrawable
{
protected:
	EDrawPriority mDrawPriority = DP_BACKGROUND;

public:
	virtual ~IDrawable() = default;
	virtual bool Draw(const PRenderer* Renderer) const = 0;

	virtual bool DebugDraw(const PRenderer* Renderer) const
	{
		return true;
	}

	EDrawPriority GetDrawPriority() const
	{
		return mDrawPriority;
	}
};
