#include "Renderer.h"

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	SDL_SetRenderDrawColor(mRenderer, R, G, B, A);
}

void PRenderer::DrawLine(float X1, float Y1, float X2, float Y2)
{
	SDL_RenderLine(mRenderer, X1, Y1, X2, Y2);
}

void PRenderer::DrawRect(FRect& Rect)
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderRect(mRenderer, &SRect);
}
void PRenderer::DrawFillRect(FRect& Rect)
{
	SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderFillRect(mRenderer, &SRect);
}

int32_t PRenderer::GetScreenWidth() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Width;
}

int32_t PRenderer::GetScreenHeight() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Height;
}