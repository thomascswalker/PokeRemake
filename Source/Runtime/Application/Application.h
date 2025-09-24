#pragma once

#include "Core/CoreFwd.h"
#include "Core/Time.h"
#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "SDL3/SDL.h"

// Used by SDL_Window unique pointer
struct SDLWindowDestroyer
{
	void operator()(SDL_Window* Window) const
	{
		SDL_DestroyWindow(Window);
	}
};

// Used by SDL_Window unique pointer
struct SDLRendererDestroyer
{
	void operator()(SDL_Renderer* Renderer) const
	{
		SDL_DestroyRenderer(Renderer);
	}
};

class PApplication
{
	static PApplication*	 sInstance;
	std::unique_ptr<PEngine> mEngine;
	Time::TimePoint			 mCurrentTime;
	bool					 bIsEditor = false;

protected:
	PApplication() = default;

public:
	static PApplication* GetInstance();

	bool Initialize(SDL_WindowFlags WindowFlags, const std::string& GPUMode, bool IsEditor);
	void Uninitialize() const;

	template <typename GameType>
	bool Start()
	{
		if (!GEngine->Init<GameType>())
		{
			LogError("Failed to initialize engine.");
			return false;
		}

		if (!GEngine->PreStart())
		{
			return false;
		}
		if (!GEngine->Start())
		{
			return false;
		}

		while (IsRunning())
		{
			if (!Loop())
			{
				break;
			}
		}

		if (!Stop())
		{
			LogError("Failed to stop application.");
		}

		return true;
	}

	bool Stop() const
	{
		GEngine->Stop();
		GEngine->~PEngine();
		GEngine = nullptr;
		return true;
	}

	bool Loop();

	/* Events */

	bool HandleEvent(void* Event);
	bool Draw(float DeltaTime) const;

	/* Properties */

	bool IsRunning() const;
};

PApplication* GetApplication();

#define CREATE_APP(GameType)                                            \
	const auto Args = ArgParser::Parse(argc, argv);                     \
	const auto App = PApplication::GetInstance();                       \
                                                                        \
	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor)) \
	{                                                                   \
		if (!App->Start<GameType>())                                    \
		{                                                               \
			LogError("Failed to start application.");                   \
			return 1;                                                   \
		}                                                               \
	}                                                                   \
	else                                                                \
	{                                                                   \
		return 1;                                                       \
	}                                                                   \
                                                                        \
	return 0;
