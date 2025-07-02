#pragma once
#include "Layout.h"
#include "Widget.h"

class PCanvas : public PWidget
{
public:
	PCanvas() {}
	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(PColor::UIBackground);
		Renderer->DrawFillRect(GetGeometry());
	}
};