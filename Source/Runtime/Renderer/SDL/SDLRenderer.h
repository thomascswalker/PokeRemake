#pragma once

#include "SDL3/SDL.h"

#include <Renderer/Generic/IRenderer.h>

class SDLRenderer : public IRenderer
{
	SDL_Renderer* mRenderer;

	float mZoomFactor = 10.0f;

public:
	explicit SDLRenderer(SDL_Renderer* Renderer) : mRenderer(Renderer) {}

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
	void DrawLine(float X1, float Y1, float X2, float Y2) override;
	void DrawRect(FRect& Rect) override;
	void DrawFillRect(FRect& Rect) override;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mRenderer); }
	int32_t		GetScreenWidth() const override;
	int32_t		GetScreenHeight() const override;

	void  SetZoomFactor(float ZoomFactor) override { mZoomFactor = ZoomFactor; }
	float GetZoomFactor() const override { return mZoomFactor; }
};