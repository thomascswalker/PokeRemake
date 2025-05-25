#pragma once

#include "SDL3/SDL.h"

#include <Renderer/Generic/IRenderer.h>

class SDLRenderer : public IRenderer
{
	SDL_Renderer* mRenderer;

public:
	explicit SDLRenderer(SDL_Renderer* Renderer) : mRenderer(Renderer) {}

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) override;
	void DrawLine(float X1, float Y1, float X2, float Y2) override;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mRenderer); }
	int32_t		GetScreenWidth() const override;
	int32_t		GetScreenHeight() const override;
};