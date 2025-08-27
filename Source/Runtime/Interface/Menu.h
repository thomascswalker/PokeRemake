#pragma once

#include "AbstractPopup.h"
#include "Panel.h"
#include "Button.h"
#include "Engine/World.h"

class PMenuView;
class PMenu;

struct SMenuItemData
{
    int32_t Index;
    void* Data;
};

DECLARE_MULTICAST_DELEGATE(DMenuItemClicked, SMenuItemData*);

class PMenuView : public PPanel
{
    PMenu* mMenu                = nullptr;
    DMenuItemClicked* mDelegate = nullptr;

public:
    PMenuView(const std::vector<std::string>& InStrings, DMenuItemClicked* InDelegate);
    void OnMouseEvent(SInputEvent* Event) override;
    void OnItemClicked();

    friend class PMenu;
};

class PMenu : public PButton
{
    std::vector<std::string> mItems;
    PMenuView* mView;
    DMenuItemClicked ItemClicked;

    void OnItemClicked(SMenuItemData* Data);

public:
    PMenu(const std::string& Name, const std::vector<std::string>& InItems);

    void AddItem(const std::string& Item);
    void ShowView(bool State);
    void HideView();
};

class PMenuBar : public PPanel
{
    TArray<PMenu*> mMenus;

public:
    PMenuBar();

    PMenu* AddMenu(const std::string& Name, const std::vector<std::string>& InItems);
};
