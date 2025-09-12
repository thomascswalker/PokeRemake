#include "Menu.h"

#include "Divider.h"

PMenuView::PMenuView(std::vector<SMenuItemData>* InData, DMenuItemClicked* InDelegate)
	: mDelegate(InDelegate),
	  mData(InData)
{
	SetFloating(true);
	SetVisible(false);
	mLayoutMode = LM_Vertical;
	mResizeModeW = RM_Grow;
	mResizeModeH = RM_Fit;
	Padding = { 0 };

	// Add each item to the view
	for (int32_t Index = 0; Index < mData->size(); Index++)
	{
		auto Item = &InData->at(Index);
		Item->Index = Index;

		// Construct a separator
		if (Item->IsSeparator)
		{
			PHDivider* Separator = ConstructWidget<PHDivider>();
			PWidget::AddChild(Separator);
		}
		else
		{
			// Otherwise construct the button for this menu item
			PButton* Button = ConstructWidget<PButton>(Item->Name);
			Button->SetResizeModeW(RM_Grow);
			Button->SetResizeModeH(RM_Fixed);
			Button->SetFixedHeight(20);
			Button->Padding = { 0 };

			// Set this button's custom data to the current item
			Button->SetCustomData(Item);

			// Callback for when the menu item is clicked, we broadcast the delegate
			// passed in from the PMenu parent.
			Button->Clicked.AddLambda([InDelegate] {
				InDelegate->Broadcast();
			});
			PWidget::AddChild(Button);
		}
	}
}

void PMenuView::OnMouseEvent(SInputEvent* Event)
{
	bool OldMouseOver = mMouseOver;
	mMouseOver = GetGeometry().Contains(Event->MousePosition);
	bool ParentOver = mMenu->GetGeometry().Contains(Event->MousePosition);
	if (OldMouseOver && !mMouseOver && !ParentOver)
	{
		HoverEnd.Broadcast();
		Event->Consume();
	}
}

void PMenuView::OnItemClicked()
{
	if (!mDelegate)
	{
		LogError("Delegate is invalid.");
		return;
	}

	mDelegate->Broadcast();
}

void PMenu::OnItemClicked()
{
	auto Data = GetSender()->GetCustomData<SMenuItemData>();
	SetCustomData(Data);
	Data->Clicked.Broadcast();
	HideView();
}

PMenu::PMenu(const std::string& Name, const std::vector<SMenuItemData>& InItems)
	: PButton(Name, this, &PMenu::ShowView), mItems(InItems)
{
	mResizeModeW = RM_Fixed;
	mFixedSize.X = 80;
	mCheckable = true;

	ItemClicked.AddRaw(this, &PMenu::OnItemClicked);

	mView = GetWorld()->ConstructWidget<PMenuView>(&mItems, &ItemClicked);
	mView->SetVisible(false, true);
	mView->mMenu = this;
}

void PMenu::AddItem(const SMenuItemData& Item)
{
	mItems.emplace_back(Item);
}

void PMenu::ShowView(bool State)
{
	if (mItems.empty())
	{
		return;
	}
	mView->SetVisible(true, true);
	PWidget::AddChild(mView);
	mView->HoverEnd.AddRaw(this, &PMenu::HideView);
	MenuOpened.Broadcast(this);
}

void PMenu::HideView()
{
	mView->SetVisible(false, true);
	PWidget::RemoveChild(mView);
	mChecked = false;
	MenuClosed.Broadcast(this);
}

PMenuBar::PMenuBar()
{
	mResizeModeW = RM_Grow;
	mResizeModeH = RM_Fit;
}

PMenu* PMenuBar::AddMenu(const std::string& Name, const std::vector<SMenuItemData>& InItems)
{
	auto Menu = ConstructWidget<PMenu>(Name, InItems);
	Menu->HoverBegin.AddRaw(this, &PMenuBar::OnMenuHoverBegin);
	Menu->MenuOpened.AddRaw(this, &PMenuBar::OnMenuOpened);
	Menu->MenuClosed.AddRaw(this, &PMenuBar::OnMenuClosed);
	mMenus.Add(Menu);
	PWidget::AddChild(Menu);
	return Menu;
}

void PMenuBar::OnMenuOpened(PMenu* Menu)
{
	if (mOpenMenu)
	{
		mOpenMenu->SetChecked(false);
		mOpenMenu->HideView();
	}
	mOpenMenu = Menu;
}

void PMenuBar::OnMenuClosed(PMenu* Menu)
{
	for (auto M : mMenus)
	{
		if (M == Menu)
		{
			continue;
		}
		M->SetChecked(false);
	}
	mOpenMenu = nullptr;
}

void PMenuBar::OnMenuHoverBegin()
{
	auto Menu = dynamic_cast<PMenu*>(GetSender());

	// If there is no open menu OR the menu we are now hovering is the current menu
	// do an early exit.
	if (!mOpenMenu || mOpenMenu == Menu)
	{
		return;
	}

	// Hide the current menu
	mOpenMenu->HideView();
	mOpenMenu = nullptr;
}
