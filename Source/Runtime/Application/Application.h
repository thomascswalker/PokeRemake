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

	std::unique_ptr<PRenderer> mRenderer;
	SDL_Window*				   mSDLWindow;
	SDL_Renderer*			   mSDLRenderer;
	std::unique_ptr<IRHI>	   mRHI;

protected:
	PApplication() = default;

public:
	static PApplication* GetInstance();

	bool Initialize(SDL_WindowFlags WindowFlags);
	void Uninitialize() const;

	template <typename GameType>
	void Start() const
	{
		if (mEngine)
		{
			mEngine->StartGame<GameType>();
			if (PGame* Game = mEngine->GetGame())
			{
				Game->PreStart();
				Game->Start();
			}
		}
	}

	void Loop();
	/* Events */

	bool OnEvent(void* Event);
	void OnDraw() const;

	/* Properties */

	bool	   IsRunning() const;
	PEngine*   GetEngine() const;
	PRenderer* GetRenderer() const;

	/* Input */
	void OnKeyDown(uint32_t ScanCode) override;
	void OnKeyUp(uint32_t ScanCode) override;
	void OnMiddleMouseScroll(float Delta) override;
};
