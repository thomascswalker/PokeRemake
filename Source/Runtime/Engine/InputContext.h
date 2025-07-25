#pragma once

#include <string>
#include <vector>

using TInputContext = std::vector<SDL_Keycode>;

static TInputContext DefaultContext =
{
    SDLK_W,
    SDLK_S,
    SDLK_A,
    SDLK_D,
    SDLK_E,
    SDLK_Q
};

static TInputContext DialogContext =
{
    SDLK_E
};
