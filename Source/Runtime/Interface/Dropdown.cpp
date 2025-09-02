#include "Dropdown.h"

#include "Text.h"

PDropdownView::PDropdownView(const std::vector<std::string>& InStrings)
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

		SDropdownItemData ItemData(Index);
		Button->SetCustomData(&ItemData);
		Button->Clicked.AddRaw(this, &PDropdownView::OnItemClicked);
		PWidget::AddChild(Button);
	}
}

void PDropdownView::OnMouseEvent(SInputEvent* Event)
{
	bool OldMouseOver = mMouseOver;
	mMouseOver        = GetGeometry().Contains(Event->MousePosition);
	bool ParentOver   = mDropdown->GetGeometry().Contains(Event->MousePosition);
	if (OldMouseOver && !mMouseOver && !ParentOver)
	{
		HoverEnd.Broadcast();
	}
	Event->Consume();
}

void PDropdownView::OnItemClicked()
{
	auto Sender   = GetSender();
	auto Dropdown = dynamic_cast<PDropdown*>(GetParent());
	if (!Dropdown)
	{
		LogError("DropdownView parent is not a dropdown!");
		return;
	}
	Dropdown->ItemClicked.Broadcast(Sender->GetCustomData<SDropdownItemData>());
}

PDropdown::PDropdown()
	: PButton(this, &PDropdown::ShowDropdownView), mCurrentIndex(0)
{
	mText->SetText("");
	mCheckable = true;

	mDropdownView = GetWorld()->ConstructWidget<PDropdownView>(mItems);
	mDropdownView->SetVisible(false);

	ItemClicked.AddRaw(this, &PDropdown::OnItemClicked);
	HoverEnd.AddRaw(this, &PDropdown::HideDropdownView);
}

PDropdown::PDropdown(const std::vector<std::string>& InItems)
	: PButton(this, &PDropdown::ShowDropdownView), mCurrentIndex(0)
{
	mCheckable = true;

	for (auto& Item : InItems)
	{
		mItems.emplace_back(Item);
	}

	if (mItems.size())
	{
		mText->SetText(mItems[mCurrentIndex]);
	}
	else
	{
		mText->SetText("");
	}

	mDropdownView = GetWorld()->ConstructWidget<PDropdownView>(mItems);
	mDropdownView->SetVisible(false);
	mDropdownView->mDropdown = this;

	ItemClicked.AddRaw(this, &PDropdown::OnItemClicked);
}

void PDropdown::Draw(const PRenderer* Renderer) const
{
	PButton::Draw(Renderer);

	float TriangleSize = 10.0f;
	auto Pos           = GetGeometry().GetPosition();
	auto Size          = GetGeometry().GetSize();
	FVector2 Offset    = Pos + Size;
	Offset.Y -= ((Size.Y / 2) + (TriangleSize / 2));
	Offset.X -= 20;

	std::vector TriangleVerts = //
	{
		FVector2{0.0f, 0.0f}
		+ Offset, //
		FVector2{TriangleSize / 2.0f, TriangleSize}
		+ Offset, //
		FVector2{TriangleSize, 0.0f}
		+ Offset //
	};
	Renderer->SetDrawColor(PColor::OffWhite);
	Renderer->DrawPolygon(TriangleVerts, {0, 1, 2});
}

void PDropdown::AddItem(const std::string& Item)
{
	mItems.emplace_back(Item);
}

void PDropdown::OnItemClicked(SDropdownItemData* Data)
{
	mCurrentIndex = Data->Index;
	mText->SetText(mItems[mCurrentIndex]);
	HideDropdownView();
}

void PDropdown::ShowDropdownView(bool State)
{
	mDropdownView->SetVisible(true);
	PWidget::AddChild(mDropdownView);
	mDropdownView->HoverEnd.AddRaw(this, &PDropdown::HideDropdownView);
}

void PDropdown::HideDropdownView()
{
	mDropdownView->SetVisible(false);
	PWidget::RemoveChild(mDropdownView);
	mChecked = false;
}
