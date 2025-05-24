#pragma once
#include <cstdint>

class IRenderer
{
public:
	virtual ~IRenderer() = default;
	virtual void DrawLine(float X1, float Y1, float X2, float Y2) = 0;

	virtual uint32_t GetScreenWidth() const = 0;
	virtual uint32_t GetScreenHeight() const = 0;
};
