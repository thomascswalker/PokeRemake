#include <format>

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

	// Store initial time
	mCurrentTime = SDL_GetTicks();

	// Construct the game engine
	mEngine = std::make_unique<Engine>();

	return true;
}

void SDLPlatform::OnStop()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	mEngine->Stop();
}

void SDLPlatform::OnLoop()
{
	// Handle events
	SDL_Event Event;
	if (SDL_PollEvent(&Event))
	{
		if (!OnEvent(&Event))
		{
			return;
		}
	}

	// Tick the engine
	const uint64_t Now = SDL_GetTicksNS();
	const uint64_t DeltaTimeNS = Now - mCurrentTime; // Get delta time in nanoseconds
	const float DeltaTime = static_cast<float>(DeltaTimeNS / 1000) / 1000.0f; // Convert to seconds
	mEngine->Tick(DeltaTime);
	mCurrentTime = Now;

	// Draw to the screen
	OnDraw();
}

bool SDLPlatform::OnEvent(void* Event)
{
	const auto* SDLEvent = static_cast<SDL_Event*>(Event);
	if (!SDLEvent)
	{
		return false;
	}

	switch (SDLEvent->type)
	{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				OnStop();
				return false;
			}
		case SDL_EVENT_KEY_DOWN:
			{
				OnKeyDown(SDLEvent->key.key);
				break;
			}
		case SDL_EVENT_KEY_UP:
			{
				OnKeyUp(SDLEvent->key.key);
				break;
			}
		default:
			break;
	}

	return true;
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

void SDLPlatform::OnKeyDown(uint32_t KeyCode) {}
void SDLPlatform::OnKeyUp(uint32_t KeyCode) {}