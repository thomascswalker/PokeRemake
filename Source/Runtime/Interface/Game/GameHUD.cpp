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
