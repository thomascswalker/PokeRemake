#pragma once
#include "Application/Application.h"

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
        SetInputContext(IC_Select);

        return true;
    }

    bool Unload() override
    {
        return true;
    }
};
