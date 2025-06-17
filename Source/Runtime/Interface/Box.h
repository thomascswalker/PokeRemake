#pragma once

#include "Widget.h"

class PBox : public PWidget
{
public:
	void Draw(const PRenderer* Renderer) const override
	{
		FRect Rect = GetGeometry();
		Renderer->SetDrawColor(255, 255, 255, 255); // White color
		Renderer->DrawRect(Rect);
		PWidget::Draw(Renderer);
	}
};