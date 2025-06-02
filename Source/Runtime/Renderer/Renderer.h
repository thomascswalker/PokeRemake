#pragma once

#include "Application/Context.h"
#include "Core/Matrix.h"
#include "Core/Rect.h"
#include "Engine/Mesh.h"
#include "Engine/Texture.h"

#include <vector>

class PRenderer
{
	SDLContext*				 mContext;
	SDL_GPUGraphicsPipeline* mPipeline;

	float	mZoomFactor = 10.0f;
	FMatrix mMVP;

public:
	explicit PRenderer(SDLContext* InContext) : mContext(InContext), mPipeline(nullptr) {}

	bool Initialize();
	void PostInitialize();
	bool Initialize3D();
	void Uninitialize() const;

	void Render();
	void Render3D();
	void Render2D();

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;
	void DrawPolygon(const std::vector<FVector2>& Vertices, const std::vector<int>& Indexes) const;

	void DrawTextureAt(PTexture* Texture, const FRect& Rect, const FVector2& Position) const;
	void DrawSpriteAt(PTexture* Texture, const FRect& Rect, const FVector2& Position,
					  int32_t Index) const;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mContext->Renderer); }

	float	 GetScreenWidth() const;
	float	 GetScreenHeight() const;
	FVector2 GetScreenSize() const;
	FRect	 GetViewport() const;

	void  SetZoomFactor(float ZoomFactor) { mZoomFactor = ZoomFactor; }
	float GetZoomFactor() const { return mZoomFactor; }
};