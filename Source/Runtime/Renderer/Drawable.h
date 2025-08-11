#pragma once

enum EZDepth
{
	Z_0      = 0, // In the background of all else
	Z_BG     = 100,
	Z_FG     = 200,
	Z_NPC    = 300,
	Z_Player = 400,
	Z_1000   = 1000, // In front of all else
};

inline std::array gDrawPriorities = {Z_0, Z_BG, Z_FG, Z_NPC, Z_Player, Z_1000};

class IDrawable
{
protected:
	EZDepth mDrawPriority = Z_0;

public:
	virtual ~IDrawable() = default;

	virtual bool Draw(const PRenderer* Renderer) const
	{
		return true;
	}

	virtual bool DebugDraw(const PRenderer* Renderer) const
	{
		return true;
	}

	EZDepth GetDrawPriority() const
	{
		return mDrawPriority;
	}

	virtual void SetDrawPriority(const EZDepth Priority)
	{
		mDrawPriority = Priority;
	}
};
