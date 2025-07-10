#pragma once

#include "Engine/InputManager.h"
#include "Engine/World.h"
#include "Widget.h"

// An item wrapper around some arbitrary data and its corresponding display widget.
class PAbstractItem
{
	PWidget* mWidget;
	void*	 mData;

public:
	PAbstractItem() = default;
	PAbstractItem(PWidget* Widget)
		: mWidget(Widget), mData(nullptr) {}

	template <typename T>
	T* GetWidget() const
	{
		return dynamic_cast<T*>(mWidget);
	}

	template <typename T>
	T* GetData() const
	{
		return dynamic_cast<T*>(mData);
	}

	template <typename T>
	void SetData(T* Data)
	{
		auto Size = sizeof(T);
		mData = std::malloc(Size);
		std::memcpy(mData, Data, Size);
	}
};

class PAbstractView : public PWidget
{
	std::vector<PAbstractItem> mItems;

	float mScrollValue = 0.0f;
	float mScrollSpeed = 20.0f;

public:
	PAbstractView()
	{
		mLayoutMode = LM_Vertical;
		mResizeModeH = RM_Grow;
	}

	bool OnMouseEvent(SInputEvent* Event) override
	{
		if (Event->Type != IET_MouseScroll)
		{
			return false;
		}
		if (GetGeometry().Contains(Event->MousePosition))
		{
			SetScrollValue(Event->MouseScroll);
			Event->Consume();
			return true;
		}
		return false;
	}

	// Adds a new item to this view. This will instantiate a new widget of type T, forward
	// its constructor arguments, and add that widget as a child of this widget.
	template <typename T, typename... ArgsType>
	PAbstractItem* AddItem(ArgsType&&... Args)
	{
		T*			   Widget = GetWorld()->ConstructWidget<T>(std::forward<ArgsType>(Args)...);
		PAbstractItem* Item = &mItems.emplace_back(Widget);
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

	void SetScrollValue(float Value)
	{
		// Only update when the mouse is over this widget.
		if (!mMouseOver)
		{
			return;
		}

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