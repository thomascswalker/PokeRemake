#pragma once

#include <Core/Delegate.h>
#include <stdint.h>

DECLARE_MULTICAST_DELEGATE(DKeyDown, uint32_t);
DECLARE_MULTICAST_DELEGATE(DKeyUp, uint32_t);

class IInputManager
{
public:
	DKeyDown KeyDown;
	DKeyUp	 KeyUp;

	virtual ~IInputManager() = default;
	virtual void OnKeyDown(uint32_t KeyCode) = 0;
	virtual void OnKeyUp(uint32_t KeyCode) = 0;
};

IInputManager* GetInputManager();
void		   SetInputManager(IInputManager* InputManager);