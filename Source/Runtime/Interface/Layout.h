#pragma once

#include <vector>

#include "Widget.h"

namespace Layout
{
	inline void SizeGrowDirection(const PWidget* Child)
	{
	}

	// Grow this widget's children to fit the space of this widget
	inline void SizeGrowChildren(const PWidget* Parent)
	{
		const auto LayoutMode = Parent->GetLayoutMode();
		auto	   ChildCount = Parent->GetChildCount();
		auto	   Growable = Parent->GetGrowableChildren(LayoutMode);
		auto	   GrowableCount = Growable.size();

		// Track the remaining width and height we can grow to within this widget
		float RemainingWidth = Parent->W;
		float RemainingHeight = Parent->H;

		// Remove width and height with the widget's padding
		RemainingWidth -= Parent->Padding.Left + Parent->Padding.Right;
		RemainingHeight -= Parent->Padding.Top + Parent->Padding.Bottom;

		// Remove width/height (depending on the layout mode) for each
		// child within this widget (as long as it's visible).
		for (auto Child : Parent->GetChildren())
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

		// Remove width/height for the gap between each child
		if (LayoutMode == LM_Horizontal)
		{
			RemainingWidth -= (ChildCount - 1) * Parent->Padding.Right;
		}
		else
		{
			RemainingHeight -= (ChildCount - 1) * Parent->Padding.Bottom;
		}

		const float GridWidth = LayoutMode == LM_Grid ? RemainingWidth / static_cast<float>(Parent->GetGridCount()) : 0;

		// Grow the children to fit the remaining width/height
		for (auto Child : Parent->GetChildren())
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
						Child->W += RemainingWidth / GrowableCount;
					}
					if (Child->GetResizeModeH() == RM_Grow)
					{
						Child->H += RemainingHeight - Child->H;
					}
					break;
				case LM_Vertical:
					if (Child->GetResizeModeH() == RM_Grow)
					{
						Child->H += RemainingHeight / GrowableCount;
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
				default:
					break;
			}
			SizeGrowChildren(Child);
		}
	}

	inline void SizeFit(PWidget* Widget)
	{
		// Fit children prior to fitting this widget
		for (const auto Child : Widget->GetChildren())
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			SizeFit(Child);
		}

		const auto RMW = Widget->GetResizeModeW();
		const auto RMH = Widget->GetResizeModeH();

		if (RMW == RM_Fit || RMH == RM_Fit)
		{
			if (RMW == RM_Fit)
			{
				Widget->W += Widget->Padding.Right;
			}
			if (RMH == RM_Fit)
			{
				Widget->H += Widget->Padding.Bottom;
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
				SumW += Child->W + Widget->Padding.Right;
				SumH += Child->H + Widget->Padding.Bottom;
				LargestW = std::max(LargestW, Child->W + Widget->Padding.Right);
				LargestH = std::max(LargestH, Child->H + Widget->Padding.Bottom);
			}

			switch (Widget->GetLayoutMode())
			{
				case LM_Horizontal:
					if (RMW == RM_Fit)
					{
						Widget->W = SumW + Widget->Padding.Right;
					}
					if (RMH == RM_Fit)
					{
						Widget->H = LargestH + Widget->Padding.Bottom;
					}
					break;
				case LM_Vertical:
					if (RMW == RM_Fit)
					{
						Widget->W = LargestW + Widget->Padding.Right;
					}
					if (RMH == RM_Fit)
					{
						Widget->H = SumH + Widget->Padding.Bottom;
					}
					break;
				case LM_Grid:
					break;
			}
		}
	}

	inline void SizeFixed(PWidget* Widget)
	{
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
			SizeFixed(Child);
		}
	}

	inline void Position(const PWidget* Widget)
	{
		const auto Children = Widget->GetChildren();

		// Full geometry of the parent widget
		const FRect Rect = Widget->GetGeometry();

		// Top-left
		const FVector2 Origin = Rect.GetPosition();

		// Uniform padding
		const auto Padding = Widget->Padding;

		// Change in X and Y as we lay out children
		float DX = Widget->Padding.Left;
		float DY = Widget->Padding.Top;

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

			switch (LayoutMode)
			{

				case LM_Horizontal:
					DX += Child->Padding.Left;
					break;
				case LM_Vertical:
					DY += Child->Padding.Top;
					break;
				case LM_Grid:
				default:
					break;
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
					DX += Child->W + Padding.Right;
					break;
				case LM_Vertical:
					DY += Child->H + Padding.Bottom;
					break;
				case LM_Grid:
					DX += Child->W;
					// Move to next row
					if (DX >= Rect.W)
					{
						DY += Child->H + Padding.Bottom;
						DX = Padding.Right;
					}
				default:
					break;
			}
		}
	}

	inline void PositionOffset(PWidget* Widget)
	{
		const auto Children = Widget->GetChildren();

		Widget->X += Widget->GetOffset().X;
		Widget->Y += Widget->GetOffset().Y;

		for (const auto Child : Children)
		{
			if (!Child->GetVisible())
			{
				continue;
			}
			PositionOffset(Child);
		}
	}

	inline void Open(PWidget* Widget)
	{
	}

	inline void Close(PWidget* Widget)
	{
	}

	inline void Layout(PWidget* Widget)
	{
		if (!Widget->GetVisible())
		{
			return;
		}

		// Size
		SizeFixed(Widget);
		SizeGrowChildren(Widget);
		SizeFit(Widget);

		// Position
		Position(Widget);
		PositionOffset(Widget);

		// Once all widgets have been recursively sized and positioned, call
		// the OnLayout function.
		Widget->OnLayout();
	}
} // namespace Layout
