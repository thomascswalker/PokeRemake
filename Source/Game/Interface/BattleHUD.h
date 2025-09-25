#pragma once
#include "Interface/Widget.h"

class PBattleHUD : public PWidget
{
	void Draw(const PRenderer* Renderer) const override
	{
		auto ScreenSize = Renderer->GetScreenSize();
		Renderer->SetDrawColor(255, 255, 255, 255);

		FVector2 Position(ScreenSize.X / 2, ScreenSize.Y / 2);
		Renderer->DrawText("Battle Mode", Position, 16.0f);
	}
};