#pragma once

#include "Widget.h"

// An item wrapper around some arbitrary data and its corresponding display widget.
struct SViewItem
{
	PWidget* Widget;
	void*	 Data;

	SViewItem() = default;
	SViewItem(PWidget* InWidget)
		: Widget(InWidget), Data(nullptr) {}

	template <typename T>
	T* GetWidget() const
	{
		return dynamic_cast<T*>(Widget);
	}

	template <typename T>
	T* GetData() const
	{
		return dynamic_cast<T*>(Data);
	}

	template <typename T>
	void SetData(T* InData)
	{
		auto Size = sizeof(T);
		Data = std::malloc(Size);
		std::memcpy(Data, InData, Size);
	}
};