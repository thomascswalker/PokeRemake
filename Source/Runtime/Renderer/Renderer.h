#pragma once

#include "Application/Context.h"
#include "Core/Matrix.h"
#include "Core/Rect.h"
#include "Engine/Texture.h"

#include <vector>

class PRenderer
{
	SDLContext*				 mContext;
	SDL_GPUGraphicsPipeline* mPipeline;

	FMatrix mMVP;

public:
	explicit PRenderer(SDLContext* InContext) : mContext(InContext), mPipeline(nullptr) {}

	bool Initialize();
	void PostInitialize() const;
	bool Initialize3D();
	void Uninitialize() const;

	void Render();
	void Render3D();
	void Render2D() const;

	FVector2 WorldToScreen(const FVector2& Position) const;

	/* Drawing */

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawPoint(const FVector2& V, float Thickness = 0.0f) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;
	void DrawPolygon(const std::vector<FVector2>& Vertices, const std::vector<int>& Indexes) const;
	void DrawGrid() const;

	void DrawPointAt(const FVector2& Position, float Thickness = 0.0f) const;
	void DrawLineAt(const FVector2& Start, const FVector2& End) const;
	void DrawRectAt(const FRect& Rect, const FVector2& Position) const;
	void DrawFillRectAt(const FRect& Rect, const FVector2& Position) const;
	void DrawTextureAt(PTexture* Texture, const FRect& Rect, const FVector2& Position) const;
	void DrawSpriteAt(PTexture* Texture, const FRect& Rect, const FVector2& Position,
					  int32_t Index) const;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mContext->Renderer); }

	float	 GetScreenWidth() const;
	float	 GetScreenHeight() const;
	FVector2 GetScreenSize() const;
	FRect	 GetViewport() const;
};