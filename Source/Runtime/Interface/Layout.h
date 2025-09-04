#pragma once

#include <vector>

#include "Widget.h"

namespace Layout
{

	// Grow this widget's children to fit the space of this widget
	inline void Grow(const PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}
		const auto LayoutMode = Widget->GetLayoutMode();
		float	   ChildGap = Widget->mPadding.Left;
		auto	   ChildCount = Widget->GetChildCount();

		// Track the remaining width and height we can grow to within this widget
		float RemainingWidth = Widget->W;
		float RemainingHeight = Widget->H;

		// Remove width and height with the widget's padding
		RemainingWidth -= Widget->mPadding.Left + Widget->mPadding.Right;
		RemainingHeight -= Widget->mPadding.Top + Widget->mPadding.Bottom;

		// Remove width/height (depending on the layout mode) for each
		// child within this widget (as long as it's visible).
		for (auto Child : Widget->GetChildren())
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			switch (LayoutMode)
			{
				case LM_Horizontal:
					RemainingWidth -= Child->W;
					break;
				case LM_Vertical:
					RemainingHeight -= Child->H;
					break;
				case LM_Grid:
					break;
			}
		}

		// // Remove width/height for the gap between each child
		// if (LayoutMode == LM_Horizontal)
		// {
		// 	RemainingWidth -= (ChildCount - 1) * ChildGap;
		// }
		// else
		// {
		// 	RemainingHeight -= (ChildCount - 1) * ChildGap;
		// }

		const float GridWidth = LayoutMode == LM_Grid ? RemainingWidth / static_cast<float>(Widget->GetGridCount()) : 0;

		// Grow the children to fit the remaining width/height
		for (auto Child : Widget->GetChildren())
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			switch (LayoutMode)
			{
				case LM_Horizontal:
					if (Child->GetResizeModeW() == RM_Grow)
					{
						Child->W += RemainingWidth;
					}
					if (Child->GetResizeModeH() == RM_Grow)
					{
						Child->H += RemainingHeight - Child->H;
					}
					break;
				case LM_Vertical:
					if (Child->GetResizeModeH() == RM_Grow)
					{
						Child->H += RemainingHeight;
					}
					if (Child->GetResizeModeW() == RM_Grow)
					{
						Child->W += RemainingWidth - Child->W;
					}
					break;
				case LM_Grid:
					if (Child->GetResizeModeH() == RM_Grow)
					{
						Child->H += GridWidth;
					}
					if (Child->GetResizeModeW() == RM_Grow)
					{
						Child->W += GridWidth;
					}
					break;
			}
			Grow(Child);
		}
	}

	inline void Fit(PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		const auto RMW = Widget->GetResizeModeW();
		const auto RMH = Widget->GetResizeModeH();

		if (RMW == RM_Fit || RMH == RM_Fit)
		{
			const float Padding = Widget->mPadding.Left;

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
				if (!Child->GetVisible())
				{
					continue;
				}
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
				case LM_Grid:
					break;
			}
		}

		for (const auto Child : Widget->GetChildren())
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			Fit(Child);
		}
	}

	inline void Fixed(PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		// All widgets initialize at their fixed size.

		const auto Size = Widget->GetFixedSize();
		Widget->W = Size.X;
		Widget->H = Size.Y;

		for (const auto Child : Widget->GetChildren())
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			Fixed(Child);
		}
	}

	inline void Position(const PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		const auto Children = Widget->GetChildren();

		// Full geometry of the parent widget
		const FRect Rect = Widget->GetGeometry();

		// Top-left
		const FVector2 Origin = { Rect.X, Rect.Y };

		// Uniform padding
		const float Padding = Widget->mPadding.Left;

		// Change in X and Y as we lay out children
		float DX = Padding;
		float DY = Padding;

		const auto LayoutMode = Widget->GetLayoutMode();

		for (const auto Child : Children)
		{
			if (!Child->GetVisible())
			{
				continue;
			}

			if (Child->GetFloating())
			{
				Child->X = Rect.X;
				Child->Y = Rect.Y + Rect.H;
				Position(Child);
				continue;
			}

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
				case LM_Grid:
					DX += Child->W;
					// Move to next row
					if (DX >= Rect.W)
					{
						DY += Child->H + Padding;
						DX = Padding;
					}
					break;
			}
		}
	}

	inline void Offset(PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		const auto Children = Widget->GetChildren();

		Widget->X += Widget->GetOffset().X;
		Widget->Y += Widget->GetOffset().Y;

		for (const auto Child : Children)
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			Offset(Child);
		}
	}

	inline void Layout(PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		Fixed(Widget);
		Fit(Widget);
		Grow(Widget);
		Position(Widget);
		Offset(Widget);

		// Once all widgets have been recursively sized and positioned, call
		// the OnLayout function.
		Widget->OnLayout();
	}
} // namespace Layout
