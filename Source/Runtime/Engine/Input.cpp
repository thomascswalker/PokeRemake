#include "Input.h"

bool SInputEvent::ShiftDown = false;
bool SInputEvent::ControlDown = false;
bool SInputEvent::AltDown = false;

static IInputManager* gInputManager = nullptr;

IInputManager* GetInputManager() { return gInputManager; }
void		   SetInputManager(IInputManager* InputManager) { gInputManager = InputManager; }