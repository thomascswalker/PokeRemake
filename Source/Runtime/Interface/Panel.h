#pragma once

#include "Box.h"

class PPanel : public PBox
{
public:
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
