#include "Menu.h"

PMenuView::PMenuView(std::vector<SMenuItemData>* InData, DMenuItemClicked* InDelegate)
	: mDelegate(InDelegate),
	  mData(InData)
{
	mLayoutMode  = LM_Vertical;
	mResizeModeW = RM_Grow;
	mResizeModeH = RM_Fit;
	mFloating    = true;
	mPadding     = {0};

	// Add each item to the view
	for (int32_t Index = 0; Index < mData->size(); Index++)
	{
		auto Item       = &InData->at(Index);
		PButton* Button = GetWorld()->ConstructWidget<PButton>(Item->Name);
		Button->SetResizeModeW(RM_Grow);
		Button->SetResizeModeH(RM_Fixed);
		Button->SetFixedHeight(20);
		Button->mPadding = {0};

		Item->Index = Index;
		Button->SetCustomData(Item);
		Button->Clicked.AddLambda([=]
		{
			InDelegate->Broadcast();
		});
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
	: PButton(this, &PMenu::ShowView), mItems(InItems)
{
	mPadding     = {0};
	mResizeModeW = RM_Fixed;
	mFixedSize.X = 80;
	mText        = Name;
	mCheckable   = true;

	HoverEnd.AddRaw(this, &PMenu::HideView);
	ItemClicked.AddRaw(this, &PMenu::OnItemClicked);

	mView = GetWorld()->ConstructWidget<PMenuView>(&mItems, &ItemClicked);
	mView->SetVisible(false);
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

PMenu* PMenuBar::AddMenu(const std::string& Name, const std::vector<SMenuItemData>& InItems)
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
