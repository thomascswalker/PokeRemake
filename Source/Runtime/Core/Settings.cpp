#include "Settings.h"

#include "Engine/InputManager.h"

void PSettings::Initialize()
{
	if (const auto Input = GetInputManager())
	{
		Input->KeyUp.AddRaw(this, &PSettings::OnKeyUp);
	}
}

void PSettings::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case KB_G:
			bDebugDraw = !bDebugDraw;
			break;
		default:
			break;
	}
}
