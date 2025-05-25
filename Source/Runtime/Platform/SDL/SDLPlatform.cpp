#include <format>

#include <SDL3/SDL.h>

#include "Core/Logging.h"
#include "SDLPlatform.h"

#define WINDOW_DEFAULT_HEIGHT 432
#define WINDOW_DEFAULT_WIDTH 480
#define WINDOW_TITLE "PokeRemake"

bool SDLPlatform::OnStart(int argc, char** argv)
{
	Info("Constructing SDLPlatform");
	SDL_SetAppMetadata(WINDOW_TITLE, "1.0", WINDOW_TITLE);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		Debug("Couldn't initialize SDL: {}", SDL_GetError());
		return false;
	}

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, 0,
			&mSDLWindow, &mSDLRenderer))
	{
		Debug("Couldn't create {}: {}", WINDOW_TITLE, SDL_GetError());
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
	mEngine = std::make_unique<PEngine>();
	gEngine = mEngine.get();
	mRenderer = std::make_unique<SDLRenderer>(mSDLRenderer);

	gEngine->GetWorld()->ConstructActor<PGrid>(20, 18, 20);

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
	gEngine = nullptr;
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
				OnKeyDown(SDLEvent->key.scancode);
				break;
			}
		case SDL_EVENT_KEY_UP:
			{
				OnKeyUp(SDLEvent->key.scancode);
				break;
			}
		case SDL_EVENT_MOUSE_WHEEL:
			{
				OnMiddleMouseScroll(SDLEvent->wheel.y);
				break;
			}
		default:
			break;
	}

	return true;
}
void SDLPlatform::OnDraw()
{
	SDL_SetRenderDrawColor(mSDLRenderer, 38, 38, 38, 255);
	SDL_RenderClear(mSDLRenderer);

	// Draw all renderables in the world
	for (IDrawable* Drawable : mEngine->GetWorld()->GetDrawables())
	{
		Drawable->Draw(mRenderer.get());
	}

	SDL_RenderPresent(mSDLRenderer);
}

bool SDLPlatform::IsRunning() { return mEngine->IsRunning(); }

void SDLPlatform::OnKeyDown(uint32_t ScanCode) { KeyDown.Broadcast(ScanCode); }
void SDLPlatform::OnKeyUp(uint32_t ScanCode) { KeyUp.Broadcast(ScanCode); }
void SDLPlatform::OnMiddleMouseScroll(float Delta) { MouseScroll.Broadcast(Delta); }
