#include "GameHUD.h"

#include "Application/Application.h"

PGameHUD::PGameHUD()
{
	SetLayoutMode(LM_Vertical);
}

void PGameHUD::DialogBox(const std::string& Text)
{
	// Create dialog box
	if (mDialogBox == nullptr)
	{
		SetInputContext(IC_Dialog);
		mDialogBox = ConstructWidget<PDialogBox>();
		PWidget::AddChild(mDialogBox);
		mDialogBox->SetText(Text);
		mDialogBox->Print();
	}
	// Destroy dialog box
	else
	{
		RestoreInputContext();
		mDialogBox->EndPrint();
		mDialogBox->Unparent();
		GetWorld()->DestroyWidget(mDialogBox);
		mDialogBox = nullptr;
	}
}
