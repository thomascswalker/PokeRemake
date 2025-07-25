#include "Input.h"

static IInputManager* gInputManager = nullptr;

IInputManager* GetInputManager() { return gInputManager; }
void		   SetInputManager(IInputManager* InputManager) { gInputManager = InputManager; }