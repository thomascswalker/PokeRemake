#pragma once
#include "Layout.h"
#include "Widget.h"

class PCanvas : public PWidget
{
public:
	PCanvas()
	{
		// GENERATE_OBJECT()
	}
	// void LayoutChildren() override
	// {
	// 	switch (mLayoutMode)
	// 	{
	// 		case WL_Horizontal:
	// 			LayoutHorizontal(mChildren, { X, Y }, WIDGET_SPACING);
	// 			break;
	// 		case WL_Vertical:
	// 			LayoutVertical(mChildren, { X, Y }, WIDGET_SPACING);
	// 			break;
	// 	}
	// }
	//
	// void Draw(const PRenderer* Renderer) const override
	// {
	// 	for (const auto& Child : mChildren)
	// 	{
	// 		Child->Draw(Renderer);
	// 	}
	// }
	//
	// void ProcessEvents(SWidgetEvent* Event) override
	// {
	// 	mSender = this;
	// 	for (const auto& Child : mChildren)
	// 	{
	// 		if (Event->bConsumed)
	// 		{
	// 			return;
	// 		}
	// 		Child->ProcessEvents(Event);
	// 	}
	// }
};