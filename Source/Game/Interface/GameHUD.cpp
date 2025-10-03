#include "GameHUD.h"

PGameHUD::PGameHUD()
{
	SetLayoutMode(LM_Vertical);
}

void PGameHUD::StartDialogBox(const std::string& Text)
{
	SetInputContext(IC_Dialog);
	mDialogBox = ConstructWidget<PDialogBox>();
	PWidget::AddChild(mDialogBox);
	mDialogBox->SetText(Text);
	mDialogBox->Print();
}

void PGameHUD::EndDialogBox()
{
	if (!mDialogBox)
	{
		return;
	}
	RestoreInputContext();
	mDialogBox->EndPrint();
	mDialogBox->Unparent();
	GWorld->DestroyWidget(mDialogBox);
	mDialogBox = nullptr;
}

bool PGameHUD::IsDialogBoxVisible()
{
	return mDialogBox != nullptr;
}

void PGameHUD::StartBattleHUD()
{
	SetInputContext(IC_Battle);

	mBattleHUD = ConstructWidget<PBattleHUD>();
	mBattleHUD->ShowActionBox();

	PWidget::AddChild(mBattleHUD);
}

void PGameHUD::EndBattleHUD()
{
	SetInputContext(IC_Default);
	mBattleHUD->Unparent();

	GWorld->DestroyWidget(mBattleHUD);
	mBattleHUD = nullptr;
}
