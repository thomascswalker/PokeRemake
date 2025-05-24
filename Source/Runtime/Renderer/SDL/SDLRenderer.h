#pragma once

#include "SDL3/SDL.h"

#include <Renderer/Generic/IRenderer.h>

class SDLRenderer : public IRenderer
{
	SDL_Renderer* mRenderer;

public:
	explicit SDLRenderer(SDL_Renderer* Renderer) : mRenderer(Renderer) {}
	void DrawLine(float X1, float Y1, float X2, float Y2) override;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mRenderer); }
	uint32_t	GetScreenWidth() const override
	{
		int32_t Width, Height;
		SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
		return Width;
	}
	uint32_t GetScreenHeight() const override
	{
		int32_t Width, Height;
		SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
		return Height;
	}
};