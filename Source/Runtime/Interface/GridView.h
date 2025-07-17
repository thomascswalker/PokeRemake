#pragma once

#include "Engine/InputManager.h"
#include "Engine/World.h"
#include "ItemView.h"
#include "Widget.h"

class PGridView : public PWidget
{
	std::vector<SViewItem> mItems;

	float mScrollValue = 0.0f;
	float mScrollSpeed = 20.0f;

public:
	PGridView()
	{
		mLayoutMode = LM_Grid;
		mResizeModeH = RM_Grow;
		mGridWidth = 3;
		Padding = { 0 };
	}

	void OnMouseEvent(SInputEvent* Event) override
	{
		if (Event->Type != IET_MouseScroll)
		{
			return;
		}
		if (GetGeometry().Contains(Event->MousePosition))
		{
			AddScrollValue(Event->MouseScroll);
			Event->Consume();
		}
	}

	// Adds a new item to this view. This will instantiate a new widget of type T, forward
	// its constructor arguments, and add that widget as a child of this widget.
	template <typename T, typename... ArgsType>
	SViewItem* AddItem(ArgsType&&... Args)
	{
		T*		   Widget = GetWorld()->ConstructWidget<T>(std::forward<ArgsType>(Args)...);
		SViewItem* Item = &mItems.emplace_back(Widget);
		this->AddChild(Item->GetWidget<T>());
		return Item;
	}

	/**
	 * The maximum scroll value is equal to the summed height and top padding of each child,
	 * plus the last child's bottom padding, minus the height of this widget.
	 *
	 *	- c = Children
	 *	- nc = Child count
	 *	- n = Nth Child
	 *
	 *	`[ c[n]->h + c[n]->pt ... nc ] + c[nc - 1]->pb - height`
	 */
	float GetMaximumScrollValue() const
	{
		float MaximumScrollValue = 0.0f;
		for (auto Child : mChildren)
		{
			MaximumScrollValue += Child->H;
			MaximumScrollValue += Child->Padding.Top;
		}
		MaximumScrollValue += mChildren[mChildren.size() - 1]->Padding.Bottom;
		return std::max(0.0f, MaximumScrollValue - H);
	}

	void AddScrollValue(float Value)
	{
		// Value comes in as +1 for a forwards scroll, -1 for a backwards scroll. Invert this
		// so a forwards scroll will scroll up, and a backwards scroll will scroll down.
		mScrollValue += -Value * mScrollSpeed;

		// Clamp the new scroll value between 0 and the maximum possible scroll value.
		mScrollValue = std::min(std::max(mScrollValue, 0.0f), GetMaximumScrollValue());
	}

	void OnLayout() override
	{
		// Update the Y offset for each child to be the inverse of the current scroll value.
		for (auto Child : mChildren)
		{
			Child->SetOffsetY(-mScrollValue, true);
		}
		PWidget::OnLayout();
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(GetGeometry());
	}

	void DrawChildren(const PRenderer* Renderer) const override
	{
		Renderer->SetClipRect(GetGeometry());
		PWidget::DrawChildren(Renderer);
		Renderer->ReleaseClipRect();
	}
};