#pragma once

#include "DialogBox.h"

#include "Interface/Box.h"
#include "Interface/HUD.h"
#include "Interface/Spacer.h"

class PGameHUD : public PHUD
{
    PSpacer* mSpacer;
    PDialogBox* mDialogBox;

public:
    PGameHUD();
    ~PGameHUD() override = default;

    void DialogBox(const std::string& Text);
};
