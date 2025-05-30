#pragma once

#include "SDL3/SDL.h"

struct SDLContext
{
	SDL_Window*	  Window;
	SDL_Renderer* Renderer;

	const char*	   GPUMode = "direct3d12";
	SDL_GPUDevice* Device;
};