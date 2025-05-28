#pragma once

#include "Core/Rect.h"

#include <cstdint>

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) = 0;
	virtual void DrawLine(float X1, float Y1, float X2, float Y2) = 0;
	virtual void DrawRect(FRect& Rect) = 0;
	virtual void DrawFillRect(FRect& Rect) = 0;

	virtual int32_t GetScreenWidth() const = 0;
	virtual int32_t GetScreenHeight() const = 0;

	virtual void  SetZoomFactor(float ZoomFactor) = 0;
	virtual float GetZoomFactor() const = 0;
};

IRenderer* GetRenderer();