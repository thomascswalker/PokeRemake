#include "Renderer.h"

#include "Engine/World.h"

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const
{
	SDL_SetRenderDrawColor(mRenderer, R, G, B, A);
}

void PRenderer::DrawLine(float X1, float Y1, float X2, float Y2) const
{
	SDL_RenderLine(mRenderer, X1, Y1, X2, Y2);
}

void PRenderer::DrawRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderRect(mRenderer, &SRect);
}

void PRenderer::DrawFillRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
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
void PRenderer::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 38, 38, 38, 255);
	SDL_RenderClear(mRenderer);

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (IDrawable* Drawable : World->GetDrawables())
		{
			Drawable->Draw(this);
		}
	}

	SDL_RenderPresent(mRenderer);
}