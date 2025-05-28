#pragma once

#include "SDL3/SDL.h"

#include "Core/Rect.h"

class PRenderer
{
	SDL_Renderer* mRenderer;

	float Renderer = 10.0f;

public:
	explicit PRenderer(SDL_Renderer* Renderer) : mRenderer(Renderer) {}

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
	void DrawLine(float X1, float Y1, float X2, float Y2);
	void DrawRect(FRect& Rect);
	void DrawFillRect(FRect& Rect);

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mRenderer); }
	int32_t		GetScreenWidth() const;
	int32_t		GetScreenHeight() const;

	void  SetZoomFactor(float ZoomFactor) { Renderer = ZoomFactor; }
	float GetZoomFactor() const { return Renderer; }
};