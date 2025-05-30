#include "Renderer.h"

#include "Core/Logging.h"
#include "Engine/World.h"

bool PRenderer::Initialize() const
{
	const auto FormatFlags =
		SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;

	LogInfo("Creating {} GPU Device", mContext->GPUMode.c_str());
	mContext->Device = SDL_CreateGPUDevice(FormatFlags, true, mContext->GPUMode.c_str());
	if (!mContext->Device)
	{
		LogError("Failed to create GPU Device: {}", SDL_GetError());
		return false;
	}

	if (!SDL_ClaimWindowForGPUDevice(mContext->Device, mContext->Window))
	{
		LogError("Failed to claim window for GPU Device: {}", SDL_GetError());
		return false;
	}

	LogInfo("Created GPU device.");
	return true;
}

void PRenderer::Uninitialize() const
{
	SDL_ReleaseWindowFromGPUDevice(mContext->Device, mContext->Window);
}

// Main render function
void PRenderer::Render()
{
	SDL_SetRenderDrawColor(mContext->Renderer, 38, 38, 38, 255);
	SDL_RenderClear(mContext->Renderer);

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (IDrawable* Drawable : World->GetDrawables())
		{
			Drawable->Draw(this);
		}
	}

	SDL_RenderPresent(mContext->Renderer);
}

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const
{
	SDL_SetRenderDrawColor(mContext->Renderer, R, G, B, A);
}

void PRenderer::DrawLine(float X1, float Y1, float X2, float Y2) const
{
	SDL_RenderLine(mContext->Renderer, X1, Y1, X2, Y2);
}

void PRenderer::DrawRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderRect(mContext->Renderer, &SRect);
}

void PRenderer::DrawFillRect(const FRect& Rect) const
{
	const SDL_FRect SRect(Rect.X, Rect.Y, Rect.W, Rect.H);
	SDL_RenderFillRect(mContext->Renderer, &SRect);
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
