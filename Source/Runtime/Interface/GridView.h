#pragma once

#include "Engine/InputManager.h"
#include "Engine/World.h"
#include "ItemView.h"
#include "Widget.h"

class PGridView : public PWidget
{
	std::vector<SViewItem> mItems;

public:
	PGridView()
	{
		mLayoutMode = LM_Grid;
		mResizeModeH = RM_Grow;
		Padding = { 0 };
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
};