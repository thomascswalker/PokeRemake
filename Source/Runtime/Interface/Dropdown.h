#pragma once

#include "AbstractButton.h"
#include "Button.h"
#include "Engine/World.h"
#include "Panel.h"

class PDropdownView;
class PDropdown;

struct SDropdownItemData
{
	int32_t Index;
};

DECLARE_MULTICAST_DELEGATE(DItemClicked, SDropdownItemData*);

class PDropdownView : public PPanel
{
public:
	PDropdownView(const std::vector<std::string>& InItems);
	void OnMouseEvent(SInputEvent* Event) override;
	void OnItemClicked();
};

class PDropdown : public PButton
{
	std::vector<std::string> mItems;
	int						 mCurrentIndex;
	PDropdownView*			 mDropdownView;
	// DelegateHandle			 mHideDelegate;

public:
	DItemClicked ItemClicked;

	PDropdown() = default;
	PDropdown(const std::vector<std::string>& InItems);

	void Draw(const PRenderer* Renderer) const override;
	void OnItemClicked(SDropdownItemData* Data);
	void ShowDropdownView(bool State);
	void HideDropdownView();
};