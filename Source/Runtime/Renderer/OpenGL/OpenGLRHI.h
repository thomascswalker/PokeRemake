#pragma once

#include "Renderer/IRHI.h"

class OpenGLRHI : public IRHI
{
	SDL_GLContext mContext = nullptr;

public:
	bool Initialize(SDL_Window* Window) override;
};
