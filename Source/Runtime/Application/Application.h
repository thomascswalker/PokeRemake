#pragma once

#include "Core/CoreFwd.h"
#include "Core/Time.h"
#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/Input.h"
#include "Renderer/Renderer.h"
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
	static PApplication* sInstance;

	Time::TimePoint mCurrentTime;

	std::shared_ptr<PEngine> mEngine;

	/* Rendering */

	std::unique_ptr<PRenderer>	mRenderer;
	std::unique_ptr<SDLContext> mContext;

	/* Editor */

	bool bIsEditor = false;

protected:
	PApplication() = default;

public:
	static PApplication* GetInstance();

	bool Initialize(SDL_WindowFlags WindowFlags, const std::string& GPUMode, bool IsEditor);
	void Uninitialize() const;

	template <typename GameType, typename HUDType = PHUD>
	bool Start() const
	{
		if (!mEngine)
		{
			LogError("Engine is invalid.");
			return false;
		}
		mEngine->Initialize<GameType>();
		mEngine->Start();

		PGame* Game = mEngine->GetGame();
		if (!Game)
		{
			LogError("Game is invalid.");
			return false;
		}

		if (!Game->Start())
		{
			LogError("Failed to start game.");
			return false;
		}
		auto World = Game->GetWorld();
		auto HUD = World->CreateHUD<HUDType>();
		HUD->PreStart();

		mRenderer->PostInitialize();

		return true;
	}

	bool Stop() const
	{
		mEngine->Stop();
		return true;
	}

	bool Loop();

	/* Events */

	bool HandleEvent(void* Event);
	bool Draw(float DeltaTime) const;

	/* Properties */

	bool		IsRunning() const;
	PEngine*	GetEngine() const;
	PRenderer*	GetRenderer() const;
	SDLContext* GetContext() const;
};

PApplication* GetApplication();

#define CREATE_APP(GameType, HUDType)                                   \
	const auto Args = ArgParser::Parse(argc, argv);                     \
	const auto App = PApplication::GetInstance();                       \
                                                                        \
	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor)) \
	{                                                                   \
		if (!App->Start<GameType, HUDType>())                           \
		{                                                               \
			LogError("Failed to start application.");                   \
			return 1;                                                   \
		}                                                               \
		while (App->IsRunning())                                        \
		{                                                               \
			if (!App->Loop())                                           \
			{                                                           \
				break;                                                  \
			}                                                           \
		}                                                               \
		if (!App->Stop())                                               \
		{                                                               \
			LogError("Failed to stop application.");                    \
			return 1;                                                   \
		}                                                               \
	}                                                                   \
	else                                                                \
	{                                                                   \
		return 1;                                                       \
	}                                                                   \
                                                                        \
	return 0;
