#pragma once

#include "Widget.h"

class PGroup : public PWidget
{
	std::string mLabel;

public:
	PGroup(const std::string& Label)
		: mLabel(Label)
	{
		Padding = { 5 };
	}

	void Draw(const PRenderer* Renderer) const override
	{
		FRect Rect = GetGeometry();
		float X0 = Rect.X;
		float Y0 = Rect.Y;
		float X1 = Rect.X + Rect.W;
		float Y1 = Rect.Y + Rect.H;

		Renderer->SetDrawColor(PColor::UIBorder);

		Renderer->DrawLine(X0, Y0, X0, Y1); // Left
		Renderer->DrawLine(X0, Y1, X1, Y1); // Bottom
		Renderer->DrawLine(X1, Y1, X1, Y0); // Right

		float TextPadding = 5;
		float TextX = X0 + TextPadding;

		Renderer->SetDrawColor(PColor::UIText);
		float Width = Renderer->DrawText(mLabel, { TextX + 20, Y0 }, 16.0f);

		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawLine(X0, Y0, TextX, Y0); // Top
		Renderer->DrawLine(TextX + Width + (TextPadding * 2), Y0, X1, Y0);

		PWidget::Draw(Renderer);
	}
};
