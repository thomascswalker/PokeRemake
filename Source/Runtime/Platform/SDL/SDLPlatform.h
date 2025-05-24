#pragma once

#include "Engine/Engine.h"
#include "Engine/InputManager.h"
#include "Platform/Generic/IPlatform.h"
#include "Renderer/SDL/SDLRenderer.h"
#include "SDL3/SDL.h"

#include <memory>

class SDLPlatform : public IPlatform, public IInputManager
{
	uint64_t mCurrentTime = 0;

	std::unique_ptr<Engine>		 mEngine;
	std::unique_ptr<SDLRenderer> mRenderer;

	SDL_Window*	  mSDLWindow = nullptr;
	SDL_Renderer* mSDLRenderer = nullptr;

public:
	/* Events */
	bool OnStart(int argc, char** argv) override;
	void OnStop() override;
	void OnLoop() override;
	bool OnEvent(void* Event) override;
	void OnDraw() override;

	/* Properties */
	bool		 IsRunning() override { return mEngine->IsRunning(); }
	PlatformType GetPlatformType() override { return PlatformType::SDL; }

	/* Input */
	void OnKeyDown(uint32_t KeyCode) override;
	void OnKeyUp(uint32_t KeyCode) override;
};
