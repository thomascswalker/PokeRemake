#pragma once

#include "Application/Context.h"
#include "Engine/InputManager.h"
#include "Engine/Object.h"

struct PSettings
{
	bool DebugDraw = false;
};

DECLARE_STATIC_GLOBAL_GETTER(Settings)