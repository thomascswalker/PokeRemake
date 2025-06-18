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

inline void Layout(const std::vector<PWidget*>& Widgets, const FRect& Rect, float Spacing,
				   ELayoutMode LayoutMode, EResizeMode ResizeMode)
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

	for (const auto& Child : Widgets)
	{
		// Set the current child's position to the current position with spacing
		Child->X = CX + Spacing;
		Child->Y = CY + Spacing;

		// Depending on the child's resize mode, set the width and/or height to the expanded
		// width/height minus the spacing.
		switch (Child->GetResizeMode())
		{
			case RM_FixedXY:
				break;
			case RM_ExpandX:
				Child->W = Width - Spacing * 2.0f;
				break;
			case RM_ExpandY:
				Child->H = Height - Spacing * 2.0f;
				break;
			case RM_ExpandXY:
				Child->W = Width - Spacing * 2.0f;
				Child->H = Height - Spacing * 2.0f;
				break;
		}

		// Depending on the layout mode of the parent widget, update the current position
		// given the new width/height of the child and spacing.
		switch (LayoutMode)
		{
			case LM_Horizontal:
				CX += Child->W + Spacing;
				break;
			case LM_Vertical:
				CY += Child->H + Spacing;
				break;
		}

		Child->LayoutChildren();
	}
}

inline void LayoutResizeX(const std::vector<PWidget*>& Widgets, const FRect& Rect, float Spacing)
{
	FVector2 Available = { Rect.W - Spacing * 2, Rect.H - Spacing * 2 };

	float CX = Rect.X;
	for (const auto& Child : Widgets)
	{
		Child->X = CX + Spacing;
		Child->Y = Rect.Y + Spacing;
		Child->W = Available.X / Widgets.size() - Spacing * 2;
		Child->H = Available.Y - Spacing * 2;
		CX += Child->W + Spacing;
		Child->LayoutChildren();
	}
}

inline void LayoutResizeY(const std::vector<PWidget*>& Widgets, const FRect& Rect, float Spacing)
{
	FVector2 Available = { Rect.W - Spacing * 2, Rect.H - Spacing * 2 };

	float CY = Rect.Y;
	for (const auto& Child : Widgets)
	{
		Child->X = Rect.X + Spacing;
		Child->Y = CY + Spacing;
		Child->W = Available.X - Spacing * 2;
		Child->H = Available.Y / Widgets.size() - Spacing * 2;
		CY += Child->H + Spacing;
		Child->LayoutChildren();
	}
}