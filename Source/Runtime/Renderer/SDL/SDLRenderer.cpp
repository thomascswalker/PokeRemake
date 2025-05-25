#include "SDLRenderer.h"

void SDLRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
	SDL_SetRenderDrawColor(mRenderer, R, G, B, A);
}

void SDLRenderer::DrawLine(float X1, float Y1, float X2, float Y2)
{
	SDL_RenderLine(mRenderer, X1, Y1, X2, Y2);
}