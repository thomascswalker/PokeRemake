#pragma once

#include <stdint.h>

class IInputManager
{
public:
	virtual ~IInputManager() = default;
	virtual void OnKeyDown(uint32_t KeyCode) = 0;
	virtual void OnKeyUp(uint32_t KeyCode) = 0;
};
