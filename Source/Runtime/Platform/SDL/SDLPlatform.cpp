//
// Created by thoma on 5/22/2025.
//

#include <SDL3/SDL.h>

#include "SDLPlatform.h"

SDL_Window*	  gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
static auto	  gWindowTitle = "PokeRemake";

bool SDLPlatform::OnStart(int argc, char** argv)
{
	SDL_SetAppMetadata(gWindowTitle, "1.0", gWindowTitle);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogDebug(SDL_LOG_PRIORITY_ERROR, "Couldn't initialize SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_CreateWindowAndRenderer(gWindowTitle, 640, 480, 0, &gWindow, &gRenderer))
	{
		SDL_LogDebug(
			SDL_LOG_PRIORITY_ERROR, "Couldn't create %s: %s", gWindowTitle, SDL_GetError());
		return false;
	}
	SDL_ShowWindow(gWindow);

	SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG, "Poll");
	bIsRunning = true;
	return true;
}

void SDLPlatform::OnStop()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	bIsRunning = false;
}

bool SDLPlatform::OnLoop()
{
	SDL_Event Event;
	if (SDL_PollEvent(&Event))
	{
		OnEvent(&Event);
	}
	OnDraw();
	return false;
}

void SDLPlatform::OnEvent(void* Event)
{
	const auto* SDLEvent = static_cast<SDL_Event*>(Event);
	if (!SDLEvent)
	{
		return;
	}

	switch (SDLEvent->type)
	{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				OnStop();
				break;
			}
		default:
			break;
	}
}
void SDLPlatform::OnDraw()
{
	const auto Now = static_cast<double>(SDL_GetTicks()) / 1000.0;
	const auto Red = static_cast<float>(0.5 + 0.5 * SDL_sin(Now));
	const auto Green = static_cast<float>(0.5 + 0.5 * SDL_sin(Now + SDL_PI_D * 2 / 3));
	const auto Blue = static_cast<float>(0.5 + 0.5 * SDL_sin(Now + SDL_PI_D * 4 / 3));

	SDL_SetRenderDrawColorFloat(gRenderer, Red, Green, Blue, SDL_ALPHA_OPAQUE_FLOAT);
	SDL_RenderClear(gRenderer);
	SDL_RenderPresent(gRenderer);
}