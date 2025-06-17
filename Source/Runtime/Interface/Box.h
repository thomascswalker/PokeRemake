#pragma once

#include "Widget.h"

class PBox : public PWidget
{
public:
	void LayoutChildren() override
	{
		float CY = Y;
		for (const auto& Child : mChildren)
		{
			Child->X = X + WIDGET_SPACING;
			Child->Y = CY + WIDGET_SPACING;
			CY += Child->H + WIDGET_SPACING;
			Child->LayoutChildren();
		}
		Y += CY + WIDGET_SPACING;
	}
};