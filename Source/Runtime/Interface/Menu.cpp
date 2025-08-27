#include "Menu.h"

PMenuView::PMenuView(const std::vector<std::string>& InStrings, DMenuItemClicked* InDelegate) : mDelegate(InDelegate)
{
	mLayoutMode  = LM_Vertical;
	mResizeModeW = RM_Grow;
	mResizeModeH = RM_Fit;
	mFloating    = true;

	// Add each item to the view
	for (int32_t Index = 0; Index < InStrings.size(); Index++)
	{
		auto Item       = InStrings[Index];
		PButton* Button = GetWorld()->ConstructWidget<PButton>(Item);
		Button->SetResizeModeW(RM_Grow);
		Button->SetResizeModeH(RM_Fixed);
		Button->SetFixedHeight(20);
		SMenuItemData ItemData(Index);
		Button->SetCustomData(&ItemData);
		Button->Clicked.AddRaw(this, &PMenuView::OnItemClicked);
		PWidget::AddChild(Button);
	}
}

void PMenuView::OnMouseEvent(SInputEvent* Event)
{
	bool OldMouseOver = mMouseOver;
	mMouseOver        = GetGeometry().Contains(Event->MousePosition);
	bool ParentOver   = mMenu->GetGeometry().Contains(Event->MousePosition);
	if (OldMouseOver && !mMouseOver && !ParentOver)
	{
		HoverEnd.Broadcast();
	}
	Event->Consume();
}

void PMenuView::OnItemClicked()
{
	if (!mDelegate)
	{
		LogError("Delegate is invalid.");
		return;
	}
	auto Sender = GetSender();
	mDelegate->Broadcast(Sender->GetCustomData<SMenuItemData>());
}

void PMenu::OnItemClicked(SMenuItemData* Data)
{
	LogDebug("{}", Data->Index);
	HideView();
}

PMenu::PMenu(const std::string& Name, const std::vector<std::string>& InItems)
	: PButton(this, &PMenu::ShowView)
{
	mPadding     = {0};
	mResizeModeW = RM_Fixed;
	mFixedSize.X = 80;
	mText        = Name;
	mCheckable   = true;

	for (auto& Item : InItems)
	{
		mItems.emplace_back(Item);
	}

	mView = GetWorld()->ConstructWidget<PMenuView>(mItems, &ItemClicked);
	mView->SetVisible(false);
	mView->mMenu = this;

	HoverEnd.AddRaw(this, &PMenu::HideView);
	ItemClicked.AddRaw(this, &PMenu::OnItemClicked);
}

void PMenu::AddItem(const std::string& Item)
{
	mItems.emplace_back(Item);
}

void PMenu::ShowView(bool State)
{
	mView->SetVisible(true);
	PWidget::AddChild(mView);
	mView->HoverEnd.AddRaw(this, &PMenu::HideView);
}

void PMenu::HideView()
{
	mView->SetVisible(false);
	PWidget::RemoveChild(mView);
	mChecked = false;
}

PMenuBar::PMenuBar()
{
	mPadding = {0};

	mResizeModeW = RM_Grow;
	mResizeModeH = RM_Fit;
}

PMenu* PMenuBar::AddMenu(const std::string& Name, const std::vector<std::string>& InItems)
{
	auto Menu = ConstructWidget<PMenu>(Name, InItems);
	if (!Menu)
	{
		LogError("Failed to construct menu: {}", Name.c_str());
		return nullptr;
	}
	mMenus.Add(Menu);
	PWidget::AddChild(Menu);
	return Menu;
}
