#include "GameHUD.h"

#include "Application/Application.h"

PGameHUD::PGameHUD()
{
	SetLayoutMode(LM_Vertical);

	mSpacer = ConstructWidget<PSpacer>();
	mSpacer->SetFixedHeight(260);
	mSpacer->SetResizeModeH(RM_Fixed);

	mDialogBox = ConstructWidget<PDialogBox>();
	mDialogBox->SetVisible(false);

	// PWidget::AddChild(mSpacer);
	PWidget::AddChild(mDialogBox);
}

void PGameHUD::DialogBox(const std::string& Text)
{
	mDialogBox->ToggleVisible();
	bool isVisible = mDialogBox->GetVisible();
	isVisible
		? SetInputContext(IC_Dialog)
		: SetInputContext(IC_Default);

	mDialogBox->SetText(Text);
	isVisible ? mDialogBox->Print() : mDialogBox->EndPrint();
}
