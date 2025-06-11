#pragma once
#include "Widget.h"

#include <vector>

inline void LayoutVertical(const std::vector<PWidget*>& Widgets, const FVector2& Position,
						   float Spacing)
{
	float CY = Position.Y;
	for (const auto& Child : Widgets)
	{
		Child->X = Position.X + Spacing;
		Child->Y = CY + Spacing;
		CY += Child->H + Spacing;
		Child->LayoutChildren();
	}
}

inline void LayoutHorizontal(const std::vector<PWidget*>& Widgets, const FVector2& Position,
							 float Spacing)
{
	float CX = Position.X;
	for (const auto& Child : Widgets)
	{
		Child->X = CX + Spacing;
		Child->Y = Position.Y + Spacing;
		CX += Child->W + Spacing;
		Child->LayoutChildren();
	}
}