#include "Character.h"

void PCharacter::Draw(IRenderer* Renderer)
{
	auto Width = Renderer->GetScreenWidth();
	auto Height = Renderer->GetScreenHeight();

	auto HalfWidth = Width / 2;
	auto HalfHeight = Height / 2;

	auto ZoomFactor = Renderer->GetZoomFactor();
	auto X0 = HalfWidth - ZoomFactor;
	auto Y0 = HalfHeight - ZoomFactor;
	auto X1 = HalfWidth + ZoomFactor;
	auto Y1 = HalfHeight + ZoomFactor;

	FRect Rect(X0, Y0, X1 - X0, Y1 - Y0);
	Renderer->SetDrawColor(255, 0, 0, 255);
	Renderer->DrawFillRect(Rect);
}