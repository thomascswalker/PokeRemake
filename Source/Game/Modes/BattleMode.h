#pragma once

#include "Engine/GameMode.h"

class PBattleMode : public PGameMode
{
public:
    std::string GetName() override
    {
        return "BattleMode";
    }

    bool Load() override;
    bool Unload() override;

    void OnKeyUp(SInputEvent* Event) override;
};
