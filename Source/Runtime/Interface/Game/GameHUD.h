#pragma once

#include "Interface/Box.h"
#include "Interface/HUD.h"
#include "Interface/Spacer.h"

class PGameHUD : public PHUD
{
    PSpacer mSpacer;
    PBox mDialogBox;

public:
    PGameHUD();
    ~PGameHUD() override = default;

    void DialogBox(const std::string& Text);
};
