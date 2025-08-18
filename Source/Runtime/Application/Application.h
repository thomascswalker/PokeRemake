#pragma once

#include "Core/CoreFwd.h"
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

	uint64_t mCurrentTime = 0;

	std::shared_ptr<PEngine> mEngine;

	/* Rendering */

	std::unique_ptr<PRenderer> mRenderer;
	std::unique_ptr<SDLContext> mContext;

	/* Input */
	SInputContext mInputContext = DefaultInputContext;

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
		if (mEngine)
		{
			mEngine->Start<GameType>();
			if (PGame* Game = mEngine->GetGame())
			{
				Game->GetWorld()->CreateHUD<HUDType>();
				if (!Game->PreStart())
				{
					return false;
				}
				Game->Start();
				mRenderer->PostInitialize();
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool Loop();

	/* Events */

	bool HandleEvent(void* Event);
	bool Draw() const;

	/* Properties */

	bool IsRunning() const;
	PEngine* GetEngine() const;
	PRenderer* GetRenderer() const;
	SDLContext* GetContext() const;

	SInputContext* GetInputContext()
	{
		return &mInputContext;
	}

	void SetInputContext(const SInputContext& Context)
	{
		mInputContext = Context;
	}
};

PApplication* GetApplication();
SInputContext* GetInputContext();
void SetInputContext(SInputContext& Context);

#define CREATE_APP(GameType, HUDType) \
const auto Args = ArgParser::Parse(argc, argv); \
const auto App  = PApplication::GetInstance(); \
\
if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor)) \
{ \
	App->Start<GameType, HUDType>(); \
	while (App->IsRunning()) \
	{ \
		if (!App->Loop()) \
		{ \
			break; \
		}\
	}\
}\
else\
{\
	return 1;\
}\
\
return 0;
