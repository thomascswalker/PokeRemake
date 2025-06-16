#include <format>

#include <SDL3/SDL.h>

#include "Application.h"

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

	return true;
}
void PApplication::InitializeKeyStates()
{
	mKeyStates[SDLK_1] = false;
	mKeyStates[SDLK_2] = false;
	mKeyStates[SDLK_3] = false;
	mKeyStates[SDLK_4] = false;
	mKeyStates[SDLK_5] = false;
	mKeyStates[SDLK_6] = false;
	mKeyStates[SDLK_7] = false;
	mKeyStates[SDLK_8] = false;
	mKeyStates[SDLK_9] = false;
	mKeyStates[SDLK_0] = false;
	mKeyStates[SDLK_A] = false;
	mKeyStates[SDLK_B] = false;
	mKeyStates[SDLK_C] = false;
	mKeyStates[SDLK_D] = false;
	mKeyStates[SDLK_E] = false;
	mKeyStates[SDLK_F] = false;
	mKeyStates[SDLK_G] = false;
	mKeyStates[SDLK_H] = false;
	mKeyStates[SDLK_I] = false;
	mKeyStates[SDLK_J] = false;
	mKeyStates[SDLK_K] = false;
	mKeyStates[SDLK_L] = false;
	mKeyStates[SDLK_M] = false;
	mKeyStates[SDLK_N] = false;
	mKeyStates[SDLK_O] = false;
	mKeyStates[SDLK_P] = false;
	mKeyStates[SDLK_Q] = false;
	mKeyStates[SDLK_R] = false;
	mKeyStates[SDLK_S] = false;
	mKeyStates[SDLK_T] = false;
	mKeyStates[SDLK_U] = false;
	mKeyStates[SDLK_V] = false;
	mKeyStates[SDLK_W] = false;
	mKeyStates[SDLK_X] = false;
	mKeyStates[SDLK_Y] = false;
	mKeyStates[SDLK_Z] = false;
	mKeyStates[SDLK_SPACE] = false;
	mKeyStates[SDLK_RETURN] = false;
	mKeyStates[SDLK_ESCAPE] = false;
	mKeyStates[SDLK_BACKSPACE] = false;
	mKeyStates[SDLK_TAB] = false;
	mKeyStates[SDLK_CAPSLOCK] = false;
	mKeyStates[SDLK_LSHIFT] = false;
	mKeyStates[SDLK_RSHIFT] = false;
	mKeyStates[SDLK_LCTRL] = false;
	mKeyStates[SDLK_RCTRL] = false;
	mKeyStates[SDLK_LALT] = false;
	mKeyStates[SDLK_RALT] = false;
	mKeyStates[SDLK_INSERT] = false;
	mKeyStates[SDLK_DELETE] = false;
	mKeyStates[SDLK_HOME] = false;
	mKeyStates[SDLK_END] = false;
	mKeyStates[SDLK_PAGEUP] = false;
	mKeyStates[SDLK_PAGEDOWN] = false;
	mKeyStates[SDLK_UP] = false;
	mKeyStates[SDLK_DOWN] = false;
	mKeyStates[SDLK_LEFT] = false;
	mKeyStates[SDLK_RIGHT] = false;
	mKeyStates[SDLK_F1] = false;
	mKeyStates[SDLK_F2] = false;
	mKeyStates[SDLK_F3] = false;
	mKeyStates[SDLK_F4] = false;
	mKeyStates[SDLK_F5] = false;
	mKeyStates[SDLK_F6] = false;
	mKeyStates[SDLK_F7] = false;
	mKeyStates[SDLK_F8] = false;
	mKeyStates[SDLK_F9] = false;
	mKeyStates[SDLK_F10] = false;
	mKeyStates[SDLK_F11] = false;
	mKeyStates[SDLK_F12] = false;
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
				mKeyStates[SDLEvent->key.key] = true;
				KeyDown.Broadcast(SDLEvent->key.key);
				break;
			}
		case SDL_EVENT_KEY_UP:
			{
				mKeyStates[SDLEvent->key.key] = false;
				KeyUp.Broadcast(SDLEvent->key.key);
				break;
			}
		case SDL_EVENT_MOUSE_WHEEL:
			{
				MouseScroll.Broadcast(SDLEvent->wheel.y);
				break;
			}
		case SDL_EVENT_MOUSE_MOTION:
			{
				MouseMotion.Broadcast(SDLEvent->motion.x, SDLEvent->motion.y);
				break;
			}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				switch (SDLEvent->button.button)
				{
					case SDL_BUTTON_LEFT:
						MouseLeftClick.Broadcast();
						break;
					case SDL_BUTTON_RIGHT:
						MouseRightClick.Broadcast();
						break;
					case SDL_BUTTON_MIDDLE:
						MouseMiddleClick.Broadcast();
						break;
					default:
						break;
				}
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

bool PApplication::IsKeyDown(uint32_t KeyCode) const
{
	const auto Iter = mKeyStates.find(KeyCode);
	if (Iter == mKeyStates.end())
	{
		return false;
	}
	return Iter->second;
}

bool PApplication::IsKeyUp(uint32_t KeyCode) const
{
	const auto Iter = mKeyStates.find(KeyCode);
	if (Iter == mKeyStates.end())
	{
		return false;
	}
	return !Iter->second;
}
