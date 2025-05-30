#pragma once

#include "SDL3/SDL.h"

#include <string>

struct SDLContext
{
	SDL_Window*	   Window;
	SDL_Renderer*  Renderer;
	std::string	   GPUMode;
	SDL_GPUDevice* Device;
};