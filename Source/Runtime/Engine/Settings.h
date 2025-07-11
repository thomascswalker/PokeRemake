#pragma once

#include "Application/Context.h"
#include "Engine/InputManager.h"
#include "Engine/Object.h"

class PSettings : public PObject, public IInputHandler
{

public:
	bool DebugDraw = false;

	void Tick(float DeltaTime) override {}
	bool OnKeyUp(SInputEvent* Event) override
	{
		switch (Event->KeyUp)
		{
			case SDLK_F1:
				DebugDraw = !DebugDraw;
				break;
			default:
				break;
		}
		return false;
	}
};

DECLARE_STATIC_GLOBAL_GETTER(Settings)