#include "InputContext.h"

#include "Core/Map.h"

static SInputContext gInputContext;

#define DEFAULT_EDITOR_KEYS \
SDLK_W,\
SDLK_A,\
SDLK_S,\
SDLK_D,\
SDLK_F,\
SDLK_DELETE
#define DEFAULT_EDITOR_MOUSE \
    SDL_EVENT_MOUSE_BUTTON_DOWN,\
    SDL_EVENT_MOUSE_BUTTON_UP,\
    SDL_EVENT_MOUSE_WHEEL,\
    SDL_EVENT_MOUSE_MOTION

static TMap<EInputContext, SInputContext> gInputContextMap = {
    // Game
    {
        IC_Default,
        {
            IC_Default,
            {
                SDLK_W,
                SDLK_S,
                SDLK_A,
                SDLK_D,
                SDLK_E,
                SDLK_Q
            },
            {}
        }
    },
    {
        IC_Dialog,
        {
            IC_Dialog,
            {
                SDLK_E,
            },
            {}
        }
    },
    {
        IC_Battle,
        {
            IC_Battle,
            {
                SDLK_ESCAPE
            }
        }
    },
    // Editor
    {
        IC_Select,
        {
            IC_Select,
            {DEFAULT_EDITOR_KEYS},
            {DEFAULT_EDITOR_MOUSE}
        }
    },
    {
        IC_Tile,
        {
            IC_Tile,
            {DEFAULT_EDITOR_KEYS},
            {DEFAULT_EDITOR_MOUSE}
        }
    },
    {
        IC_Actor,
        {
            IC_Actor,
            {DEFAULT_EDITOR_KEYS},
            {DEFAULT_EDITOR_MOUSE}
        }
    }
};

void SetInputContext(EInputContext Type)
{
    gInputContext = gInputContextMap[Type];
}

SInputContext* GetInputContext()
{
    return &gInputContext;
}

bool IsInputContext(EInputContext Type)
{
    return gInputContext.Type == Type;
}
