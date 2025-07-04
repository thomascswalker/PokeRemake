#pragma once

#include "Box.h"
#include "Layout.h"
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

	PBox mClipArea;

public:
	PAbstractView()
	{
		mLayoutMode = LM_Vertical;
		mResizeModeH = RM_Grow;
	}

	template <typename T, typename... ArgsType>
	PAbstractItem* AddItem(ArgsType... Args)
	{
		auto Item = &mItems.emplace_back(new T(std::forward<ArgsType>(Args)...));
		PWidget::AddChild(Item->template GetWidget<T>());
		return Item;
	}

	void DrawChildren(const PRenderer* Renderer) const override
	{
		for (auto& Item : mItems)
		{
			Item.GetWidget<PWidget>()->SetOffsetY(-28, true);
		}

		Renderer->SetClipRect(GetGeometry());
		PWidget::DrawChildren(Renderer);
		Renderer->ReleaseClipRect();
	}
};