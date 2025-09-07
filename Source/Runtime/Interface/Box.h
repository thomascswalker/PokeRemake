#pragma once

#include "Widget.h"

class PBox : public PWidget
{
public:
	void Draw(const PRenderer* Renderer) const override
	{
		FRect Rect = GetGeometry();
		Renderer->SetDrawColor(Style.Primary);
		Renderer->DrawFillRect(Rect);
		Renderer->SetDrawColor(Style.Border); // White color
		Renderer->DrawRect(Rect);
		PWidget::Draw(Renderer);
	}
};
