#pragma once

#include <vector>
#include "Core/CoreFwd.h"

enum EInputContext
{
    // Game
    IC_Default = 0,
    IC_Dialog,
    IC_Battle,
    // Editor
    IC_Select = 100,
    IC_Tile,
    IC_Actor,
    // Widgets
    IC_Text = 200,
};

struct SInputContext
{
    EInputContext Type;
    std::vector<int> Keyboard;
    std::vector<int> Mouse;
    bool Any = false;
};

static SInputContext DefaultInputContext{IC_Default, {}, {}, true};

void SetInputContext(EInputContext Type);
SInputContext* GetInputContext();
bool IsInputContext(EInputContext Type);
void RestoreInputContext();
