#pragma once
#include "Engine/GameMode.h"

class PEditorMode : public PGameMode
{
public:
    std::string GetName() override
    {
        return "EditorMode";
    }

    bool Load() override
    {
        return true;
    }

    bool Unload() override
    {
        return true;
    }
};
