#pragma once

#include "Application/Context.h"

#include <stdint.h>

struct PSettings
{
	bool mDebugDraw = false;

	void Initialize();
	void OnKeyUp(uint32_t KeyCode);
};

DECLARE_STATIC_GLOBAL_GETTER(Settings)