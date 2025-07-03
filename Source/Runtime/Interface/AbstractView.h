#pragma once

#include "Widget.h"

class PAbstractItem
{
	PWidget* mWidget;

public:
	PAbstractItem() = default;
	PAbstractItem(PWidget* Widget)
		: mWidget(Widget) {}

	template <typename T>
	T* GetWidget() const
	{
		return dynamic_cast<T*>(mWidget);
	}
};

class PAbstractView : public PWidget
{
	std::vector<PAbstractItem> mItems;

public:
	PAbstractView()
	{
		mLayoutMode = LM_Vertical;
		mResizeModeH = RM_Fit;
	}

	template <typename T, typename... ArgsType>
	PAbstractItem* AddItem(ArgsType... Args)
	{
		auto Item = &mItems.emplace_back(new T(std::forward<ArgsType>(Args)...));
		PWidget::AddChild(Item->template GetWidget<T>());
		return Item;
	}
};