#pragma once

#include "Engine/InputManager.h"
#include "Layout.h"
#include "Widget.h"

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

	float GetMaximumScrollValue() const
	{
		float MaximumScrollValue = 0.0f;
		for (auto& Item : mItems)
		{
			auto Widget = Item.GetWidget<PWidget>();
			MaximumScrollValue += Widget->H;
			MaximumScrollValue += Widget->Padding.Top;
		}
		return MaximumScrollValue + H;
	}

	void SetScrollValue(float Value)
	{
		if (mMouseOver)
		{
			mScrollValue += -Value * mScrollSpeed;
			mScrollValue = std::min(std::max(mScrollValue, 0.0f), GetMaximumScrollValue());
		}
	}

	void DrawChildren(const PRenderer* Renderer) const override
	{
		for (auto& Item : mItems)
		{
			Item.GetWidget<PWidget>()->SetOffsetY(-mScrollValue, true);
		}

		Renderer->SetClipRect(GetGeometry());
		PWidget::DrawChildren(Renderer);
		Renderer->ReleaseClipRect();
	}
};