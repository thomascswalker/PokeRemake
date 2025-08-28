#pragma once

#include "AbstractPopup.h"
#include "Panel.h"
#include "Button.h"
#include "Engine/World.h"

using TMenuItemAction = std::function<void()>;

class PMenuView;
class PMenu;

struct SMenuItemData;

DECLARE_MULTICAST_DELEGATE(DMenuItemClicked);

struct SMenuItemData
{
    DMenuItemClicked Clicked;
    int32_t Index    = 0;
    std::string Name = "";
    bool IsSeparator = false;

    SMenuItemData() : IsSeparator(true) {}

    template <typename T>
    SMenuItemData(const std::string& InName, T* InObject, void (T::*Delegate)()) :
        Name(InName)
    {
        Clicked.AddRaw(InObject, Delegate);
    }
};

class PMenuView : public PPanel
{
    PMenu* mMenu                      = nullptr;
    DMenuItemClicked* mDelegate       = nullptr;
    std::vector<SMenuItemData>* mData = nullptr;

public:
    PMenuView(std::vector<SMenuItemData>* InData, DMenuItemClicked* InDelegate);
    void OnMouseEvent(SInputEvent* Event) override;
    void OnItemClicked();

    friend class PMenu;
};

class PMenu : public PButton
{
    std::vector<SMenuItemData> mItems;
    PMenuView* mView;
    DMenuItemClicked ItemClicked;

    void OnItemClicked();

public:
    PMenu(const std::string& Name, const std::vector<SMenuItemData>& InItems);

    void AddItem(const SMenuItemData& Item);
    void ShowView(bool State);
    void HideView();
};

class PMenuBar : public PPanel
{
    TArray<PMenu*> mMenus;

public:
    PMenuBar();

    PMenu* AddMenu(const std::string& Name, const std::vector<SMenuItemData>& InItems);
};
