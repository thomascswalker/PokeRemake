#pragma once

#include "SDL3/SDL.h"

#include "Core/Rect.h"
#include "IRHI.h"

class PRenderer
{
	// The SDL Renderer used for drawing
	SDL_Renderer* mRenderer;

	float Renderer = 10.0f;

public:
	explicit PRenderer(SDL_Renderer* InRenderer) : mRenderer(InRenderer) {}
	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mRenderer); }
	int32_t		GetScreenWidth() const;
	int32_t		GetScreenHeight() const;

	void  SetZoomFactor(float ZoomFactor) { Renderer = ZoomFactor; }
	float GetZoomFactor() const { return Renderer; }

	void Render();
};