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
// DEFINE_STATIC_GLOBAL_AND_GETTER(CameraView, Game);
// DEFINE_STATIC_GLOBAL_AND_GETTER(Settings, Game);

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

	// Create the engine
	mEngine = std::make_unique<PEngine>();
	GEngine = mEngine.get();

	// Create the renderer
	GRenderer = GEngine->GetGameInstance()->ConstructObject<PRenderer>();

	// Create the SDL Context wrapper
	LogDebug("Creating new SDL Window with flags: {:x}", WindowFlags);
	if (!SDL_CreateWindowAndRenderer(WindowTitle, WINDOW_WIDTH, WINDOW_HEIGHT,
									 WindowFlags, &GRenderer->Context.Window, &GRenderer->Context.Renderer))
	{
		LogDebug("Couldn't create {}: {}", WINDOW_TITLE, SDL_GetError());
		return false;
	}
#if _EDITOR
	SDL_SetWindowResizable(GRenderer->Context.Window, true);
#endif

	if (!GRenderer || !GRenderer->Initialize())
	{
		LogError("Failed to initialize renderer.");
		return false;
	}

	// Show the window
	LogDebug("Displaying window");
	SDL_ShowWindow(GRenderer->Context.Window);

	// Store initial time
	mCurrentTime = Time::Now();

	return true;
}

void PApplication::Uninitialize() const
{
	GRenderer->Uninitialize();

	LogDebug("Destroying SDL Renderer");
	SDL_DestroyRenderer(GRenderer->Context.Renderer);

	LogDebug("Destroying SDL Window");
	SDL_DestroyWindow(GRenderer->Context.Window);

	LogDebug("Stopping Engine");
	GEngine->Stop();

	LogDebug("Cleaning up all SDL subsystems");
	SDL_Quit();

	GEngine = nullptr;
	GRenderer = nullptr;

	LogDebug("Destroying {} Application", bIsEditor ? "Editor" : "Game");
}

bool PApplication::Loop()
{
	if (!GEngine->IsRunning())
	{
		return false;
	}
	auto		Now = Time::Now();
	const float DeltaTime = Time::Delta(mCurrentTime, Now); // Convert to seconds
	GEngine->Tick(DeltaTime);
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
	GEngine->PostTick();

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
				GRenderer->OnResize({ Width, Height });
			}
		case SDL_EVENT_KEY_UP:
			// Global exit across any input context
			{
				if (SDLEvent->key.key == SDLK_ESCAPE)
				{
					GEngine->GetGame()->End();
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
	GEngine->GetGame()->ProcessEvents(&InputEvent);
	return true;
}

bool PApplication::Draw(float DeltaTime) const
{
	if (!GRenderer->Render(DeltaTime))
	{
		return false;
	}
	SDL_GL_SwapWindow(GRenderer->Context.Window);
	return true;
}

bool PApplication::IsRunning() const
{
	if (!GEngine)
	{
		return false;
	}
	return GEngine->IsRunning();
}
