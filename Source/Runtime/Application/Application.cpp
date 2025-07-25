#include "Application.h"

#include <format>

#include "Core/Constants.h"
#include "Core/Logging.h"
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
	mContext          = std::make_unique<SDLContext>();
	mContext->GPUMode = GPUMode;

	LogDebug("Creating new SDL Window with flags: {:x}", WindowFlags);
	if (!SDL_CreateWindowAndRenderer(WindowTitle, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT,
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
	// Tick the engine
	const uint64_t Now         = SDL_GetTicksNS();
	const uint64_t DeltaTimeNS = Now - mCurrentTime;                               // Get delta time in nanoseconds
	const float DeltaTime      = static_cast<float>(DeltaTimeNS / 1000) / 1000.0f; // Convert to seconds
	mEngine->Tick(DeltaTime);
	mCurrentTime = Now;

	// Handle events after all events in the engine are handled
	SDL_Event Event;
	if (SDL_PollEvent(&Event))
	{
		if (!OnEvent(&Event))
		{
			return true;
		}
	}

	// Draw to the screen
	if (!OnDraw())
	{
		return false;
	}

	// Clean up any destroyable actors in the world
	mEngine->GetGame()->GetWorld()->Cleanup();

	return true;
}

bool PApplication::OnEvent(void* Event)
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
			float Width  = static_cast<float>(SDLEvent->window.data1);
			float Height = static_cast<float>(SDLEvent->window.data2);
			mRenderer->OnResize({Width, Height});
		}
	default: break;
	}

	// Handle all other events
	SInputEvent InputEvent(SDLEvent);
	if (!InputEvent.Validate(&mInputContext))
	{
		return false;
	}

	// Handle game-level events
	if (GetGame()->ProcessEvents(&InputEvent))
	{
		return true;
	}
	// Handle world-level events
	GetWorld()->ProcessEvents(&InputEvent);

	return true;
}

bool PApplication::OnDraw() const
{
	if (!mRenderer->Render())
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

SInputContext* GetInputContext()
{
	return PApplication::GetInstance()->GetInputContext();
}

void SetInputContext(const SInputContext& Context)
{
	PApplication::GetInstance()->SetInputContext(Context);
}
