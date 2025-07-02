#pragma once

#include <vector>

#include "Widget.h"

// Grow this widget to the size of its parent
// TODO: Account for multiple children that can grow
inline void LayoutGrow(PWidget* Widget)
{
	const PWidget* Parent = Widget->GetParent();
	const auto	   RMW = Widget->GetResizeModeW();
	const auto	   RMH = Widget->GetResizeModeH();

	if (Parent && (RMW == RM_Grow || RMH == RM_Grow))
	{
		const float Padding = Widget->Padding.Left;

		if (RMW == RM_Grow)
		{
			const float RemainingWidth = Parent->W;
			Widget->W = RemainingWidth - (Padding * 2);
		}

		if (RMH == RM_Grow)
		{
			const float RemainingHeight = Parent->H;
			Widget->H = RemainingHeight - (Padding * 2);
		}
	}

	for (const auto Child : Widget->GetChildren())
	{
		LayoutGrow(Child);
	}
}

inline void LayoutFitSize(PWidget* Widget)
{
	const auto RMW = Widget->GetResizeModeW();
	const auto RMH = Widget->GetResizeModeH();

	if (RMW == RM_Fit || RMH == RM_Fit)
	{
		const float Padding = Widget->Padding.Left;

		if (RMW == RM_Fit)
		{
			Widget->W += Padding;
		}
		if (RMH == RM_Fit)
		{
			Widget->H += Padding;
		}

		float SumW = 0;
		float SumH = 0;
		float LargestW = 0;
		float LargestH = 0;

		for (const auto Child : Widget->GetChildren())
		{
			SumW += Child->W + Padding;
			SumH += Child->H + Padding;
			LargestW = std::max(LargestW, Child->W + Padding);
			LargestH = std::max(LargestH, Child->H + Padding);
		}

		switch (Widget->GetLayoutMode())
		{
			case LM_Horizontal:
				if (RMW == RM_Fit)
				{
					Widget->W = SumW + Padding;
				}
				if (RMH == RM_Fit)
				{
					Widget->H = LargestH + Padding;
				}
				break;
			case LM_Vertical:
				if (RMW == RM_Fit)
				{
					Widget->W = LargestW + Padding;
				}
				if (RMH == RM_Fit)
				{
					Widget->H = SumH + Padding;
				}
				break;
		}
	}

	for (const auto Child : Widget->GetChildren())
	{
		LayoutFitSize(Child);
	}
}

inline void LayoutFixedSize(PWidget* Widget)
{
	// All widgets initialize at their fixed size.

	const auto FixedSize = Widget->GetFixedSize();
	Widget->W = FixedSize.X;
	Widget->H = FixedSize.Y;

	for (const auto Child : Widget->GetChildren())
	{
		LayoutFixedSize(Child);
	}
}

inline void LayoutPosition(const PWidget* Widget)
{
	const auto Children = Widget->GetChildren();

	// Full geometry of the parent widget
	const FRect Rect = Widget->GetGeometry();

	// Top-left
	const FVector2 Origin = { Rect.X, Rect.Y };

	// Uniform padding
	const float Padding = Widget->Padding.Left;

	// Change in X and Y as we lay out children
	float DX = Padding;
	float DY = Padding;

	const auto LayoutMode = Widget->GetLayoutMode();

	for (const auto Child : Children)
	{
		// // Child position is at the origin plus the current offset in X/Y
		Child->X = Origin.X + DX;
		Child->Y = Origin.Y + DY;

		// Recursively call this positioning
		LayoutPosition(Child);

		// Child size is the automatic size minus padding for bottom and right
		switch (LayoutMode)
		{
			case LM_Horizontal:
				DX += Child->W + Padding;
				break;
			case LM_Vertical:
				DY += Child->H + Padding;
				break;
		}
	}
}

inline void Layout(PWidget* Widget)
{
	LayoutFixedSize(Widget);
	LayoutFitSize(Widget);
	LayoutGrow(Widget);
	LayoutPosition(Widget);
}