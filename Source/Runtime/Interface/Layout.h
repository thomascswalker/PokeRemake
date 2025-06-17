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

inline void LayoutResizeXY(const std::vector<PWidget*>& Widgets, const FRect& Rect, float Spacing)
{
	FVector2 Available = { Rect.W - Spacing * 2, Rect.H - Spacing * 2 };

	float CX = Rect.X;
	float CY = Rect.Y;
	float W = Available.X / Widgets.size();
	float H = Available.Y / Widgets.size();

	for (const auto& Child : Widgets)
	{
		Child->X = CX + Spacing;
		Child->Y = CY + Spacing;

		switch (Child->GetResizeMode())
		{
			case RM_FixedXY:
				break;
			case RM_ExpandX:
				Child->W = W - Spacing * 2;
				break;
			case RM_ExpandY:
				Child->H = H - Spacing * 2;
				break;
			case RM_ExpandXY:
				Child->W = W - Spacing * 2;
				Child->H = H - Spacing * 2;
		}

		// TODO: Something's wrong with the spacing
		CX += Child->W + Spacing;
		CY += Child->H + Spacing;
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