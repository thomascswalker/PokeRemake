#pragma once

#include "Engine/World.h"

#include "Button.h"
#include "Panel.h"

class PDropdownView;
class PDropdown;

struct SDropdownItemData
{
	int32_t Index;
	void*	Data;
};

DECLARE_MULTICAST_DELEGATE(DItemClicked, SDropdownItemData*);

class PDropdownView : public PPanel
{
	PDropdown* mDropdown = nullptr;

public:
	PDropdownView(const std::vector<std::string>& InStrings);
	void OnMouseEvent(SInputEvent* Event) override;
	void OnItemClicked();

	friend class PDropdown;
};

class PDropdown : public PButton
{
	std::vector<std::string> mItems;
	int						 mCurrentIndex;
	PDropdownView*			 mDropdownView;
	// DelegateHandle			 mHideDelegate;

public:
	DItemClicked ItemClicked;

	PDropdown();
	PDropdown(const std::vector<std::string>& InItems);

	void Draw(const PRenderer* Renderer) const override;
	void AddItem(const std::string& Item);
	void OnItemClicked(SDropdownItemData* Data);
	void ShowDropdownView(bool State);
	void HideDropdownView();
};