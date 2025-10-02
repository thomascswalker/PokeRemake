#include "InputContext.h"

#include "Core/Map.h"

static SInputContext gInputContext;
static SInputContext gPrevInputContext;

#define DEFAULT_EDITOR_KEYS \
	SDLK_W,                 \
		SDLK_A,             \
		SDLK_S,             \
		SDLK_D,             \
		SDLK_F,             \
		SDLK_DELETE,

#define DEFAULT_EDITOR_MOUSE       \
	SDL_EVENT_MOUSE_BUTTON_DOWN,   \
		SDL_EVENT_MOUSE_BUTTON_UP, \
		SDL_EVENT_MOUSE_WHEEL,     \
		SDL_EVENT_MOUSE_MOTION

#define _KEY(X) SDLK_##X
#define ALL_KEYS                                                                                 \
	FOR_EACH(_KEY, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z)

static TMap<EInputContext, SInputContext> gInputContextMap = {
	// Game
	{
		IC_Default,
		{ IC_Default,
	  { SDLK_W,
		SDLK_S,
		SDLK_A,
		SDLK_D,
		SDLK_E,
		SDLK_Q },
	  {} }												 },
	{ IC_Dialog,
	  { IC_Dialog,
		{
		SDLK_E,
		},
		{} }												 },
	{ IC_Battle,
	  { IC_Battle,
		{ SDLK_W,
	  SDLK_S,
	  SDLK_A,
	  SDLK_D,
	  SDLK_E,
	  SDLK_Q, SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN } } },
	// Editor
	{
		IC_Select,
		{ IC_Select,
	  { DEFAULT_EDITOR_KEYS },
	  { DEFAULT_EDITOR_MOUSE } }							 },
	{	  IC_Tile,
	  { IC_Tile,
		{ DEFAULT_EDITOR_KEYS },
		{ DEFAULT_EDITOR_MOUSE } }						   },
	{  IC_Actor,
	  { IC_Actor,
		{ DEFAULT_EDITOR_KEYS },
		{ DEFAULT_EDITOR_MOUSE } }						   },
	// Misc
	{
		// Allow any keys to be pressed on Text input
		IC_Text,
		{ IC_Text, { /* Keys */ }, { /* Mouse */ }, true }   }
};

void SetInputContext(EInputContext Type)
{
	gPrevInputContext = gInputContext;
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

void RestoreInputContext()
{
	gInputContext = gPrevInputContext;
	gPrevInputContext = gInputContextMap[IC_Default];
}
