#pragma once
#include "Layout.h"
#include "Widget.h"

class PCanvas : public PWidget
{
public:
	PCanvas() = default;
	void LayoutChildren() const override { LayoutVertical(mChildren, { X, Y }, WIDGET_SPACING); }

	void Draw(const PRenderer* Renderer) const override
	{
		for (const auto& Child : mChildren)
		{
			Child->Draw(Renderer);
		}
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		for (const auto& Child : mChildren)
		{
			if (Event->bConsumed)
			{
				return;
			}
			Child->ProcessEvents(Event);
		}
	}
};