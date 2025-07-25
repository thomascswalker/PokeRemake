#include "GameHUD.h"

#include "Engine/World.h"

PGameHUD::PGameHUD()
{
    mLayoutMode = LM_Vertical;

    PWidget::AddChild(&mSpacer);
    mDialogBox.SetVisible(false);
    mDialogBox.SetFixedHeight(200);
    mDialogBox.SetResizeModeH(RM_Fixed);

    PWidget::AddChild(&mDialogBox);
}

void PGameHUD::DialogBox(const std::string& Text)
{
    mDialogBox.GetVisible() ? mDialogBox.SetVisible(false) : mDialogBox.SetVisible(true);
}
