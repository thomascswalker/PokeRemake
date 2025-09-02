#include "InputContext.h"

#include "Core/Map.h"

static SInputContext gInputContext;
static SInputContext gPrevInputContext;

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

#define _KEY(X) SDLK_##X
#define ALL_KEYS \
    FOR_EACH(_KEY, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z)

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
    },
    // Misc
    {
        IC_Text,
        {
            IC_Text,
            {
                SDLK_A, SDLK_B, SDLK_C, SDLK_D, SDLK_E, SDLK_F,
                SDLK_G, SDLK_H, SDLK_I, SDLK_J, SDLK_K, SDLK_L, SDLK_M,
                SDLK_N, SDLK_O, SDLK_P, SDLK_Q, SDLK_R, SDLK_S,
                SDLK_T, SDLK_U, SDLK_V, SDLK_W, SDLK_X, SDLK_Y, SDLK_Z,
                SDLK_BACKSPACE, SDLK_SPACE, SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8,
                SDLK_9
            }
        }
    }
};

void SetInputContext(EInputContext Type)
{
    gPrevInputContext = gInputContext;
    gInputContext     = gInputContextMap[Type];
}

SInputContext* GetInputContext()
{
    return &gInputContext;
}

bool IsInputContext(EInputContext Type)
{
    return gInputContext.Type == Type;
}

void RestoreInputContext()
{
    LogInfo("Restoring input context...");
    gInputContext     = gPrevInputContext;
    gPrevInputContext = gInputContextMap[IC_Default];
}
