#pragma once

#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/InputManager.h"
#include "Renderer/Renderer.h"
#include "SDL3/SDL.h"

#include <memory>

// Used by SDL_Window unique pointer
struct SDLWindowDestroyer
{
	void operator()(SDL_Window* Window) const { SDL_DestroyWindow(Window); }
};

// Used by SDL_Window unique pointer
struct SDLRendererDestroyer
{
	void operator()(SDL_Renderer* Renderer) const { SDL_DestroyRenderer(Renderer); }
};

class PApplication : public IInputManager
{
	static PApplication* sInstance;

	uint64_t mCurrentTime = 0;

	std::shared_ptr<PEngine> mEngine;

	/* Rendering */

	std::unique_ptr<PRenderer>	mRenderer;
	std::unique_ptr<SDLContext> mContext;

	/* Keyboard */

	std::map<uint32_t, bool> mKeyStates;

	/* Editor */

	bool bIsEditor = false;

protected:
	PApplication() = default;

public:
	static PApplication* GetInstance();

	bool Initialize(SDL_WindowFlags WindowFlags, const std::string& GPUMode, bool IsEditor);
	void InitializeKeyStates();
	void Uninitialize() const;

	template <typename GameType>
	bool Start() const
	{
		if (mEngine)
		{
			mEngine->Start<GameType>();
			if (PGame* Game = mEngine->GetGame())
			{
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

	bool OnEvent(void* Event);
	bool OnDraw() const;

	/* Properties */

	bool		IsRunning() const;
	PEngine*	GetEngine() const;
	PRenderer*	GetRenderer() const;
	SDLContext* GetContext() const;

	bool IsKeyDown(uint32_t KeyCode) const override;
	bool IsKeyUp(uint32_t KeyCode) const override;
};
