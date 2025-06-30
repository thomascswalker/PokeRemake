#pragma once
#include "Widget.h"

#include <vector>

inline void Layout(const PWidget* Parent, const std::vector<PWidget*>& Widgets, const FRect& Rect, ELayoutMode LayoutMode)
{
	// Available size for the widgets to fit into
	const FVector2 Available = { Rect.W, Rect.H };

	float Width = 0.0f, Height = 0.0f;
	float CX = Rect.X;
	float CY = Rect.Y;

	const int Count = Widgets.size();

	// Depending on the layout mode of the parent widget, calculate the desired size for
	// each child widget.
	switch (LayoutMode)
	{
		case LM_Horizontal:
			Width = Count == 0 ? Available.X : Available.X / Count;
			Height = Available.Y;
			break;
		case LM_Vertical:
			Width = Available.X;
			Height = Count == 0 ? Available.Y : Available.Y / Count;
			break;
	}

	for (const auto Child : Widgets)
	{
		// Set the current child's position to the current position with spacing
		Child->X = CX + Child->Padding.Left;
		Child->Y = CY + Child->Padding.Top;

		if (Child->GetResizeMode() == RM_FitContent)
		{
			// Layout children prior to determining this child's size
			Child->LayoutChildren();

			FVector2 ContentSize(0, 0);
			for (const auto GrandChild : Child->GetChildren())
			{
				switch (Child->GetLayoutMode())
				{
					case LM_Horizontal:
						ContentSize.X += GrandChild->W + GrandChild->Padding.Right;
						ContentSize.Y = std::max(GrandChild->H + GrandChild->Padding.Bottom, ContentSize.Y);
						break;
					case LM_Vertical:
						ContentSize.X = std::max(GrandChild->W + GrandChild->Padding.Right, ContentSize.X);
						ContentSize.Y += GrandChild->H + GrandChild->Padding.Bottom;
						break;
				}
			}
			Child->W = std::min(ContentSize.X + Child->Padding.Right, Child->GetMaxSize().X);
			Child->H = std::min(ContentSize.Y + Child->Padding.Bottom, Child->GetMaxSize().Y);

			switch (LayoutMode)
			{
				case LM_Horizontal:
					CX += Child->W + Child->Padding.Right;
					break;
				case LM_Vertical:
					CY += Child->H + Child->Padding.Bottom;
					break;
			}

			continue;
		}

		// Depending on the child's resize mode, set the width and/or height to the expanded
		// width/height minus the spacing.
		switch (Child->GetResizeMode())
		{
			case RM_FixedXY:
				break;
			case RM_ExpandX:
				Child->W = Width - Child->Padding.Right;
				break;
			case RM_ExpandY:
				Child->H = Height - Child->Padding.Bottom;
				break;
			case RM_ExpandXY:
				Child->W = Width - Child->Padding.Right;
				Child->H = Height - Child->Padding.Bottom;
				break;
			case RM_FitContent:
				break;
		}

		Child->W = std::min(Child->W, Child->GetMaxSize().X);
		Child->H = std::min(Child->H, Child->GetMaxSize().Y);

		// Depending on the layout mode of the parent widget, update the current position
		// given the new width/height of the child and spacing.
		switch (LayoutMode)
		{
			case LM_Horizontal:
				CX += Child->W + Child->Padding.Right;
				break;
			case LM_Vertical:
				CY += Child->H + Child->Padding.Bottom;
				break;
		}

		// Layout all children of this child.
		Child->LayoutChildren();
	}
}
