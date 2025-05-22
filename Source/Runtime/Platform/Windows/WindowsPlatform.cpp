//
// Created by thoma on 5/21/2025.
//

#include "WindowsPlatform.h"

void WindowsPlatform::Start() { bIsRunning = true; }
void WindowsPlatform::Stop() {}
bool WindowsPlatform::IsRunning() { return bIsRunning; }