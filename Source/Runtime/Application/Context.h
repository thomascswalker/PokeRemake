#pragma once

#include "SDL3/SDL.h"

#include <string>

struct SRenderContext
{
	SDL_Window*	   Window;
	SDL_Renderer*  Renderer;
	SDL_GPUDevice* Device;
	std::string	   GPUMode;

	bool IsGPU() const
	{
		return GPUMode == "direct3d12" || GPUMode == "vulkan" || GPUMode == "metal";
	}
};
