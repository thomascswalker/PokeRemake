#pragma once

#include <vector>

#include "Widget.h"

namespace Layout
{

	// Grow this widget's children to fit the space of this widget
	inline void GrowChildren(const PWidget* Widget)
	{
		const auto LayoutMode = Widget->GetLayoutMode();
		float	   ChildGap = Widget->Padding.Left;
		float	   RemainingWidth = Widget->W;
		float	   RemainingHeight = Widget->H;

		RemainingWidth -= Widget->Padding.Left + Widget->Padding.Right;
		RemainingHeight -= Widget->Padding.Top + Widget->Padding.Bottom;

		for (auto Child : Widget->GetChildren())
		{
			if (LayoutMode == LM_Horizontal)
			{
				RemainingWidth -= Child->W;
			}
			else
			{
				RemainingHeight -= Child->H;
			}
		}
		if (LayoutMode == LM_Horizontal)
		{
			RemainingWidth -= (Widget->GetChildCount() - 1) * ChildGap;
		}
		else
		{
			RemainingHeight -= (Widget->GetChildCount() - 1) * ChildGap;
		}

		for (auto Child : Widget->GetChildren())
		{
			if (LayoutMode == LM_Horizontal)
			{
				if (Child->GetResizeModeW() == RM_Grow)
				{
					Child->W += RemainingWidth;
				}
				if (Child->GetResizeModeH() == RM_Grow)
				{
					Child->H += (RemainingHeight - Child->H);
				}
			}
			else
			{
				if (Child->GetResizeModeH() == RM_Grow)
				{
					Child->H += RemainingHeight;
				}
				if (Child->GetResizeModeW() == RM_Grow)
				{
					Child->W += (RemainingWidth - Child->W);
				}
			}
			GrowChildren(Child);
		}
	}

	inline void Fit(PWidget* Widget)
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
			Fit(Child);
		}
	}

	inline void Fixed(PWidget* Widget)
	{
		// All widgets initialize at their fixed size.

		const auto Size = Widget->GetFixedSize();
		Widget->W = Size.X;
		Widget->H = Size.Y;

		for (const auto Child : Widget->GetChildren())
		{
			Fixed(Child);
		}
	}

	inline void Position(const PWidget* Widget)
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
			// Child position is at the origin plus the current change in X/Y
			Child->X = Origin.X + DX;
			Child->Y = Origin.Y + DY;

			// Recursively call this positioning
			Position(Child);

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

	inline void Offset(PWidget* Widget)
	{
		const auto Children = Widget->GetChildren();

		Widget->X += Widget->GetOffset().X;
		Widget->Y += Widget->GetOffset().Y;

		for (const auto Child : Children)
		{
			Offset(Child);
		}
	}

	inline void Layout(PWidget* Widget)
	{
		Fixed(Widget);
		Fit(Widget);
		GrowChildren(Widget);
		Position(Widget);
		Offset(Widget);

		// Once all widgets have been recursively sized and positioned, call
		// the OnLayout function.
		Widget->OnLayout();
	}
} // namespace Layout
