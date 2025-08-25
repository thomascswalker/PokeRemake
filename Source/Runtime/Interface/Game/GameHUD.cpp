#include "GameHUD.h"

#include "Application/Application.h"

PGameHUD::PGameHUD()
{
    mLayoutMode = LM_Vertical;

    mSpacer = ConstructWidget<PSpacer>();
    PWidget::AddChild(mSpacer);

    mDialogBox = ConstructWidget<PDialogBox>();
    PWidget::AddChild(mDialogBox);
    mDialogBox->SetVisible(false);
}

void PGameHUD::DialogBox(const std::string& Text)
{
    mDialogBox->GetVisible() ? mDialogBox->SetVisible(false) : mDialogBox->SetVisible(true);
    mDialogBox->GetVisible()
        ? SetInputContext(IC_Dialog)
        : SetInputContext(IC_Default);
    mDialogBox->SetText(Text);
}
