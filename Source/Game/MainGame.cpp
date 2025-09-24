#include "MainGame.h"

#include "Application/Application.h"
#include "Engine/Actors/Interactable.h"
#include "Modes/BattleMode.h"
#include "Modes/MapMode.h"

bool PMainGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}

	AddGameMode<PMapMode>();
	AddGameMode<PBattleMode>();

	return true;
}

bool PMainGame::HandleGameEvent(SGameEvent& Event)
{
	auto Data = Event.GetData<SInteractData>();
	if (!mDialogBox)
	{
		ShowDialogBox(Data->Message);
	}
	else
	{
		CloseDialogBox();
	}
	return true;
}

void PMainGame::ShowDialogBox(const std::string& Text)
{
	SetInputContext(IC_Dialog);
	mDialogBox = GWorld->ConstructWidget<PDialogBox>();
	mDialogBox->SetText(Text);
	mDialogBox->Print();
	GWorld->GetRootWidget()->AddChild(mDialogBox);
}

void PMainGame::CloseDialogBox()
{
	RestoreInputContext();
	mDialogBox->Print();
	GWorld->GetRootWidget()->RemoveChild(mDialogBox);
	GWorld->DestroyWidget(mDialogBox);
	mDialogBox = nullptr;
}
