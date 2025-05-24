#include "SDLRenderer.h"

void SDLRenderer::DrawLine(float X1, float Y1, float X2, float Y2)
{
	SDL_RenderLine(mRenderer, X1, Y1, X2, Y2);
}