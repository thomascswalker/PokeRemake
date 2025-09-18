#include "Application.h"

#include <chrono>
#include <format>

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Core/Time.h"
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

PApplication* GetApplication()
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
	LogInfo("Constructing {} Application", bIsEditor ? "Editor" : "Game");

	const auto WindowTitle = bIsEditor ? WINDOW_TITLE_EDITOR : WINDOW_TITLE;
	SDL_SetAppMetadata(WindowTitle, "1.0", WindowTitle);

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		LogDebug("Couldn't initialize SDL: {}", SDL_GetError());
		return false;
	}

	// Create the SDL Context wrapper
	mContext = std::make_unique<SDLContext>();
	mContext->GPUMode = GPUMode;

	LogDebug("Creating new SDL Window with flags: {:x}", WindowFlags);
	if (!SDL_CreateWindowAndRenderer(WindowTitle, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
									 WindowFlags, &mContext->Window, &mContext->Renderer))
	{
		LogDebug("Couldn't create {}: {}", WINDOW_TITLE, SDL_GetError());
		return false;
	}
#if _EDITOR
	SDL_SetWindowResizable(mContext->Window, true);
#endif

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
	mCurrentTime = Time::Now();

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

	LogDebug("Destroying {} Application", bIsEditor ? "Editor" : "Game");
}

bool PApplication::Loop()
{
	if (!mEngine->IsRunning())
	{
		return false;
	}
	auto		Now = Time::Now();
	const float DeltaTime = Time::Delta(mCurrentTime, Now); // Convert to seconds
	mEngine->Tick(DeltaTime);
	mCurrentTime = Now;

	// Handle events after all events in the engine are handled
	SDL_Event Event;
	if (SDL_PollEvent(&Event))
	{
		if (!HandleEvent(&Event))
		{
			return true;
		}
	}

	// Draw to the screen
	if (!Draw(DeltaTime))
	{
		return false;
	}

	// Clean up any destroyable actors in the world
	mEngine->PostTick();

	return true;
}

bool PApplication::HandleEvent(void* Event)
{
	auto* SDLEvent = static_cast<SDL_Event*>(Event);
	if (!SDLEvent)
	{
		return false;
	}

	// Handle window-level events
	switch (SDLEvent->type)
	{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				Uninitialize();
				return false;
			}
		case SDL_EVENT_WINDOW_RESIZED:
			{
				float Width = static_cast<float>(SDLEvent->window.data1);
				float Height = static_cast<float>(SDLEvent->window.data2);
				mRenderer->OnResize({ Width, Height });
			}
		case SDL_EVENT_KEY_UP:
			// Global exit across any input context
			{
				if (SDLEvent->key.key == SDLK_ESCAPE)
				{
					mEngine->GetGame()->End();
				}
				break;
			}
		default:
			break;
	}

	// Handle all other events
	SInputEvent InputEvent(SDLEvent);
	if (!InputEvent.Validate(GetInputContext()))
	{
		return false;
	}

	// Handle game events
	GetGame()->ProcessEvents(&InputEvent);
	return true;
}

bool PApplication::Draw(float DeltaTime) const
{
	if (!mRenderer->Render(DeltaTime))
	{
		return false;
	}
	SDL_GL_SwapWindow(mContext->Window);
	return true;
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
