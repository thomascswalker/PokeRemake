#pragma once

#include "Application/Context.h"

#include "Core/Rect.h"

class PRenderer
{
	SDLContext*				 mContext;
	SDL_GPUGraphicsPipeline* mPipeline;

	float mZoomFactor = 10.0f;

public:
	explicit PRenderer(SDLContext* InContext) : mContext(InContext), mPipeline(nullptr) {}

	bool Initialize();
	bool Initialize3D();
	bool Initialize2D();
	void Uninitialize() const;

	void Render() const;
	void Render3D() const;
	void Render2D() const;

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mContext->Renderer); }
	int32_t		GetScreenWidth() const;
	int32_t		GetScreenHeight() const;

	void  SetZoomFactor(float ZoomFactor) { mZoomFactor = ZoomFactor; }
	float GetZoomFactor() const { return mZoomFactor; }
};