#include "GameHUD.h"

#include "Application/Application.h"

PGameHUD::PGameHUD()
{
    mLayoutMode = LM_Vertical;

    PWidget::AddChild(&mSpacer);
    mDialogBox.SetVisible(false);

    PWidget::AddChild(&mDialogBox);
}

void PGameHUD::DialogBox(const std::string& Text)
{
    mDialogBox.GetVisible() ? mDialogBox.SetVisible(false) : mDialogBox.SetVisible(true);
    mDialogBox.GetVisible()
        ? GetApplication()->SetInputContext(Game::Context::Dialog)
        : GetApplication()->SetInputContext(Game::Context::Default);
    mDialogBox.SetText(Text);
}
