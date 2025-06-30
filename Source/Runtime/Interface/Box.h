#pragma once

#include "Widget.h"

class PBox : public PWidget
{
public:
	PBox() { mResizeMode = RM_FitContent; }

	void Draw(const PRenderer* Renderer) const override
	{
		FRect Rect = GetGeometry();
		Renderer->SetDrawColor(50, 50, 50, 255); // White color
		Renderer->DrawFillRect(Rect);
		Renderer->SetDrawColor(128, 128, 128, 255); // White color
		Renderer->DrawRect(Rect);
		PWidget::Draw(Renderer);
	}
};