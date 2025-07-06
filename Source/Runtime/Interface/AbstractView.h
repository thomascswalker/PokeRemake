#pragma once

#include "Engine/InputManager.h"
#include "Layout.h"
#include "Widget.h"

// An item wrapper around some arbitrary data and its corresponding display widget.
class PAbstractItem
{
	PWidget* mWidget;
	void*	 mData;

public:
	PAbstractItem() = default;
	PAbstractItem(PWidget* Widget)
		: mWidget(Widget) {}

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

		if (auto Input = GetInputManager())
		{
			Input->MouseScroll.AddRaw(this, &PAbstractView::SetScrollValue);
		}
	}

	template <typename T, typename... ArgsType>
	PAbstractItem* AddItem(ArgsType... Args)
	{
		auto Item = &mItems.emplace_back(new T(std::forward<ArgsType>(Args)...));
		PWidget::AddChild(Item->template GetWidget<T>());
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
		if (mMouseOver)
		{
			mScrollValue += -Value * mScrollSpeed;
			mScrollValue = std::min(std::max(mScrollValue, 0.0f), GetMaximumScrollValue());
		}
	}

	void OnLayout() override
	{
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