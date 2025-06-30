#pragma once

#include "Widget.h"

class PBox : public PWidget
{
public:
	PBox() { mResizeMode = RM_FitContent; }

	void Draw(const PRenderer* Renderer) const override
	{
		FRect Rect = GetGeometry();
		Renderer->SetDrawColor(PColor::UIPanel);
		Renderer->DrawFillRect(Rect);
		Renderer->SetDrawColor(PColor::UIBorder); // White color
		Renderer->DrawRect(Rect);
		PWidget::Draw(Renderer);
	}
};