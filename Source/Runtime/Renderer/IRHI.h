#pragma once

#include "SDL3/SDL.h"

class IRHI
{
public:
	virtual ~IRHI() = default;
	virtual bool Initialize(SDL_Window* Window) = 0;
	virtual void Render(SDL_Renderer* Renderer) = 0;
};