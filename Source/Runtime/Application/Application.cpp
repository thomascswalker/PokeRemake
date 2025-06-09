#include <format>

#include <SDL3/SDL.h>

#include "Application.h"

#include "Core/Constants.h"
#include "Core/Logging.h"

#include "Engine/Actors/Character.h"
#include "SDL3/SDL_opengl.h"

PApplication* PApplication::sInstance = nullptr;

PApplication* PApplication::GetInstance()
{
	if (!sInstance)
	{
		sInstance = new PApplication();
	}
	return sInstance;
}

static PApplication* GetApplication()
{
	return PApplication::GetInstance();
}

#define DEFINE_STATIC_GLOBAL_AND_GETTER(ClassName, ParentClass)  \
	P##ClassName* g##ClassName = nullptr;                        \
	P##ClassName* Get##ClassName()                               \
	{                                                            \
		if (!g##ClassName)                                       \
		{                                                        \
			auto Parent = Get##ParentClass();                    \
			if (!Parent)                                         \
			{                                                    \
				return nullptr;                                  \
			}                                                    \
			g##ClassName = Get##ParentClass()->Get##ClassName(); \
		}                                                        \
		return g##ClassName;                                     \
	}

// Each definition requires the corresponding declaration in the header file
// using DECLARE_STATIC_GLOBAL_GETTER in Context.h
// Example.h => PExample* GetExample();
DEFINE_STATIC_GLOBAL_AND_GETTER(Engine, Application);
DEFINE_STATIC_GLOBAL_AND_GETTER(Renderer, Application);
DEFINE_STATIC_GLOBAL_AND_GETTER(Game, Engine);
DEFINE_STATIC_GLOBAL_AND_GETTER(World, Game);
DEFINE_STATIC_GLOBAL_AND_GETTER(CameraView, Game);
DEFINE_STATIC_GLOBAL_AND_GETTER(Settings, Game);

bool PApplication::Initialize(SDL_WindowFlags WindowFlags, const std::string& GPUMode,
							  bool IsEditor)
{
	bIsEditor = IsEditor;
	if (bIsEditor)
	{
		LogInfo("Constructing Editor Application");
	}
	else
	{
		LogInfo("Constructing Application");
	}

	SDL_SetAppMetadata(WINDOW_TITLE, "1.0", WINDOW_TITLE);

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		LogDebug("Couldn't initialize SDL: {}", SDL_GetError());
		return false;
	}

	// Create the SDL Context wrapper
	mContext = std::make_unique<SDLContext>();
	mContext->GPUMode = GPUMode;

	LogDebug("Creating new SDL Window with flags: {:x}", WindowFlags);
	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
									 WindowFlags, &mContext->Window, &mContext->Renderer))
	{
		LogDebug("Couldn't create {}: {}", WINDOW_TITLE, SDL_GetError());
		return false;
	}
	SDL_SetWindowResizable(mContext->Window, true);

	// Construct the game engine
	mEngine = std::make_unique<PEngine>();

	mRenderer = std::make_unique<PRenderer>(mContext.get());
	if (!mRenderer->Initialize())
	{
		LogError("Failed to initialize renderer.");
		return false;
	}

	LogDebug("Displaying window");
	SDL_ShowWindow(mContext->Window);

	// Store initial time
	mCurrentTime = SDL_GetTicks();

	// Setup input
	SetInputManager(this);

	LogInfo("Application constructed");
	return true;
}

void PApplication::Uninitialize() const
{
	mRenderer->Uninitialize();

	LogDebug("Destroying SDL Renderer");
	SDL_DestroyRenderer(mContext->Renderer);

	LogDebug("Destroying SDL Window");
	SDL_DestroyWindow(mContext->Window);

	LogDebug("Stopping Engine");
	mEngine->Stop();

	LogDebug("Cleaning up all SDL subsystems");
	SDL_Quit();
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
				break;
			}
		default:
			break;
	}

	return true;
}

void PApplication::OnDraw() const
{
	mRenderer->Render();
	SDL_GL_SwapWindow(mContext->Window);
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

SDLContext* PApplication::GetContext() const
{
	return mContext.get();
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
