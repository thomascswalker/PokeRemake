#pragma once

#include <Core/Delegate.h>
#include <stdint.h>

DECLARE_MULTICAST_DELEGATE(DKeyDown, uint32_t);
DECLARE_MULTICAST_DELEGATE(DKeyUp, uint32_t);
DECLARE_MULTICAST_DELEGATE(DMiddleMouseScroll, float);

enum PKey
{
	KB_A = 0x04,
	KB_B,
	KB_C,
	KB_D,
	KB_E,
	KB_F,
	KB_G,
	KB_H,
	KB_I,
	KB_J,
	KB_K,
	KB_L,
	KB_M,
	KB_N,
	KB_O,
	KB_P,
	KB_Q,
	KB_R,
	KB_S,
	KB_T,
	KB_U,
	KB_V,
	KB_W,
	KB_X,
	KB_Y,
	KB_Z,
	KB_1 = 0x1E,
	KB_2,
	KB_3,
	KB_4,
	KB_5,
	KB_6,
	KB_7,
	KB_8,
	KB_9,
	KB_0,
	KB_Right = 0x4F,
	KB_Left,
	KB_Down,
	KB_Up,
	KB_Escape = 0x29,
};

class IInputManager
{
public:
	DKeyDown		   KeyDown;
	DKeyUp			   KeyUp;
	DMiddleMouseScroll MouseScroll;

	virtual ~IInputManager() = default;
	virtual void OnKeyDown(uint32_t KeyCode) = 0;
	virtual void OnKeyUp(uint32_t KeyCode) = 0;
	virtual void OnMiddleMouseScroll(float Delta) = 0;
};

IInputManager* GetInputManager();
void		   SetInputManager(IInputManager* InputManager);