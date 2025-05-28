#include <format>

#include <SDL3/SDL.h>

#include "Application.h"
#include "Core/Logging.h"

#include "Engine/Actors/Character.h"
#include "Renderer/OpenGL/OpenGLRHI.h"

#define WINDOW_DEFAULT_HEIGHT 432
#define WINDOW_DEFAULT_WIDTH 480
#define WINDOW_TITLE "PokeRemake"

PApplication* PApplication::sInstance = nullptr;

PApplication* PApplication::GetInstance()
{
	if (!sInstance)
	{
		sInstance = new PApplication();
	}
	return sInstance;
}

PEngine* GetEngine()
{
	return PApplication::GetInstance()->GetEngine();
}

PRenderer* GetRenderer()
{
	return PApplication::GetInstance()->GetRenderer();
}

PGame* GetGame()
{
	if (const auto Engine = PApplication::GetInstance()->GetEngine())
	{
		return Engine->GetGame();
	}
	return nullptr;
}

PWorld* GetWorld()
{
	if (const auto Game = GetGame())
	{
		return Game->GetWorld();
	}
	return nullptr;
}

bool PApplication::Initialize(SDL_WindowFlags WindowFlags)
{
	Info("Constructing SDLPlatform");
	SDL_SetAppMetadata(WINDOW_TITLE, "1.0", WINDOW_TITLE);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		Debug("Couldn't initialize SDL: {}", SDL_GetError());
		return false;
	}

	Debug("Creating new SDL Window with flags: {:x}", WindowFlags);
	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
									 WindowFlags, &mSDLWindow, &mSDLRenderer))
	{
		Debug("Couldn't create {}: {}", WINDOW_TITLE, SDL_GetError());
		return false;
	}
	SDL_SetWindowResizable(mSDLWindow, true);
	if ((WindowFlags & SDL_WINDOW_OPENGL) != 0)
	{
		mRHI = new OpenGLRHI();
	}
	else
	{
		Error("No RHI available for the current window flags: {}", WindowFlags);
		return false;
	}
	mRenderer = std::make_unique<PRenderer>(mSDLRenderer, mRHI);

	Debug("Displaying window");
	SDL_ShowWindow(mSDLWindow);

	// Store initial time
	mCurrentTime = SDL_GetTicks();

	// Setup input
	SetInputManager(this);

	// Construct the game engine
	mEngine = std::make_unique<PEngine>();

	Info("SDLPlatform constructed");
	return true;
}

void PApplication::Uninitialize() const
{
	Debug("Destroying SDL Renderer");
	SDL_DestroyRenderer(mSDLRenderer);

	Debug("Destroying SDL Window");
	SDL_DestroyWindow(mSDLWindow);

	Debug("Stopping Engine");
	mEngine->Stop();
}

void PApplication::Loop()
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

bool PApplication::OnEvent(void* Event)
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
				Uninitialize();
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
				mRenderer->SetZoomFactor(mRenderer->GetZoomFactor() + SDLEvent->wheel.y);
				break;
			}
		default:
			break;
	}

	return true;
}

void PApplication::OnDraw() const
{
	SDL_SetRenderDrawColor(mSDLRenderer, 38, 38, 38, 255);
	SDL_RenderClear(mSDLRenderer);

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (IDrawable* Drawable : World->GetDrawables())
		{
			Drawable->Draw(mRenderer.get());
		}
	}

	SDL_RenderPresent(mSDLRenderer);
}

bool PApplication::IsRunning() const
{
	return mEngine->IsRunning();
}

PEngine* PApplication::GetEngine() const
{
	return mEngine.get();
}

PRenderer* PApplication::GetRenderer() const
{
	return mRenderer.get();
}

void PApplication::OnKeyDown(uint32_t ScanCode)
{
	KeyDown.Broadcast(ScanCode);
}
void PApplication::OnKeyUp(uint32_t ScanCode)
{
	KeyUp.Broadcast(ScanCode);
}
void PApplication::OnMiddleMouseScroll(float Delta)
{
	MouseScroll.Broadcast(Delta);
}
