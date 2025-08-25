#pragma once

#include <vector>
#include "Core/CoreFwd.h"

struct SInputContext
{
    std::string Name;
    std::vector<int> Keyboard;
    std::vector<int> Mouse;
    bool Any = false;
};

static SInputContext DefaultInputContext{"Default", {}, {}, true};

namespace Input::Context
{
    static SInputContext Default =
    {
        "Game::Default",
        {
            SDLK_W,
            SDLK_S,
            SDLK_A,
            SDLK_D,
            SDLK_E,
            SDLK_Q
        },
        {}
    };

    static SInputContext Dialog =
    {
        "Game::Dialog",
        {
            SDLK_E,
        },
        {}
    };
}

namespace Editor::Context
{
    static SInputContext Select{
        "Editor::Select",
        {},
        {
            SDL_EVENT_MOUSE_BUTTON_DOWN,
            SDL_EVENT_MOUSE_BUTTON_UP,
        },
    };
}
