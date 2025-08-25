#pragma once

#include "Engine/GameMode.h"
#include "Engine/MapManager.h"

class PMapMode : public PGameMode
{
public:
    PMapMode();

    std::string GetName() override
    {
        return "MapMode";
    }

    bool Load() override;
    bool Unload() override;
    void OnKeyUp(SInputEvent* Event) override;
};
