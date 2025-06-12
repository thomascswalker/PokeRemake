#pragma once

#include "SDL3/SDL.h"

#include <Core/Delegate.h>
#include <stdint.h>

DECLARE_MULTICAST_DELEGATE(DKeyDown, uint32_t);
DECLARE_MULTICAST_DELEGATE(DKeyUp, uint32_t);
DECLARE_MULTICAST_DELEGATE(DMiddleMouseScroll, float);
DECLARE_MULTICAST_DELEGATE(DMouseMotion, float, float);
DECLARE_MULTICAST_DELEGATE(DMouseLeftClick);
DECLARE_MULTICAST_DELEGATE(DMouseRightClick);
DECLARE_MULTICAST_DELEGATE(DMouseMiddleClick);

class IInputManager
{
public:
	DKeyDown		   KeyDown;
	DKeyUp			   KeyUp;
	DMiddleMouseScroll MouseScroll;
	DMouseMotion	   MouseMotion;
	DMouseLeftClick	   MouseLeftClick;
	DMouseRightClick   MouseRightClick;
	DMouseMiddleClick  MouseMiddleClick;

	virtual ~IInputManager() = default;
	virtual bool IsKeyDown(uint32_t KeyCode) const = 0;
	virtual bool IsKeyUp(uint32_t KeyCode) const = 0;
	bool		 IsShiftDown() const { return IsKeyDown(SDLK_LSHIFT) || IsKeyDown(SDLK_RSHIFT); }
	bool		 IsCtrlDown() const { return IsKeyDown(SDLK_LCTRL) || IsKeyDown(SDLK_RCTRL); }
	bool		 IsAltDown() const { return IsKeyDown(SDLK_LALT) || IsKeyDown(SDLK_RALT); }
};

IInputManager* GetInputManager();
void		   SetInputManager(IInputManager* InputManager);