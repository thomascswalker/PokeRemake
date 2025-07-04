#pragma once
#include "AbstractButton.h"
#include "Widget.h"
#include <vector>

class PButtonGroup : public PWidget
{
	std::vector<PAbstractButton*> mButtons;

public:
	PButtonGroup() = default;

	void AddButton(PAbstractButton* Button)
	{
		mButtons.push_back(Button);
		Button->SetButtonGroup(this);
	}

	void OnButtonChecked(bool State)
	{
		// Deselect all buttons other than the one which sent this broadcast.
		auto Sender = static_cast<PAbstractButton*>(mSender);
		for (const auto& Button : mButtons)
		{
			if (Button->GetInternalName() != Sender->GetInternalName())
			{
				Button->SetChecked(false);
			}
		}
	}
};