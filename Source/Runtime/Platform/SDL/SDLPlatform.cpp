#include <format>

#include <SDL3/SDL.h>

#include "Core/Logging.h"
#include "SDLPlatform.h"

constexpr uint32_t mWindowDefaultHeight = 480;
constexpr uint32_t mWindowDefaultWidth = 640;
static auto		   mWindowTitle = "PokeRemake";

bool SDLPlatform::OnStart(int argc, char** argv)
{
	Info("Constructing SDLPlatform");
	SDL_SetAppMetadata(mWindowTitle, "1.0", mWindowTitle);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		Debug("Couldn't initialize SDL: {}", SDL_GetError());
		return false;
	}

	if (!SDL_CreateWindowAndRenderer(
			mWindowTitle, mWindowDefaultWidth, mWindowDefaultHeight, 0, &mSDLWindow, &mSDLRenderer))
	{
		Debug("Couldn't create {}: {}", mWindowTitle, SDL_GetError());
		return false;
	}
	SDL_SetWindowResizable(mSDLWindow, true);

	Debug("Displaying window");
	SDL_ShowWindow(mSDLWindow);

	// Store initial time
	mCurrentTime = SDL_GetTicks();

	// Setup input
	SetInputManager(this);

	// Construct the game engine
	mEngine = std::make_unique<Engine>();
	mRenderer = std::make_unique<SDLRenderer>(mSDLRenderer);

	Info("SDLPlatform constructed");
	return true;
}

void SDLPlatform::OnStop()
{
	Debug("Destroying SDL Renderer");
	SDL_DestroyRenderer(mSDLRenderer);

	Debug("Destroying SDL Window");
	SDL_DestroyWindow(mSDLWindow);

	Debug("Stopping Engine");
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
	SDL_SetRenderDrawColorFloat(mSDLRenderer, Red, Green, Blue, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mSDLRenderer);

	SDL_SetRenderDrawColor(mSDLRenderer, 255, 255, 255, 255);
	mEngine->GetWorld()->Draw(mRenderer.get());

	SDL_RenderPresent(mSDLRenderer);
}

void SDLPlatform::OnKeyDown(uint32_t KeyCode) { KeyDown.Broadcast(KeyCode); }
void SDLPlatform::OnKeyUp(uint32_t KeyCode) { KeyUp.Broadcast(KeyCode); }
