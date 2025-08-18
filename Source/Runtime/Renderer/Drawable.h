#pragma once

#include "Core/CoreFwd.h"

enum EZDepth
{
	Z_0      = 0, // In the background of all else
	Z_BG     = 100,
	Z_FG     = 200,
	Z_NPC    = 300,
	Z_Player = 400,
	Z_1000   = 1000, // In front of all else
};

namespace Drawing
{
	inline float Z_0      = 0.0f;
	inline float Z_BG     = 100.0f;
	inline float Z_FG     = 200.0f;
	inline float Z_NPC    = 300.0f;
	inline float Z_Player = 400.0f;
	inline float Z_1000   = 1000.0f; // In front of all else
}

inline std::array gDrawPriorities = {Z_0, Z_BG, Z_FG, Z_NPC, Z_Player, Z_1000};

class IDrawable
{
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

	virtual float GetDepth() const = 0;
};

bool DepthSort(const PActor* A, const PActor* B);
