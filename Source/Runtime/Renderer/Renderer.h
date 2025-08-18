#pragma once

#include <vector>

#include "Core/CoreFwd.h"
#include "Engine/Texture.h"
#include "Engine/Tileset.h"
#include "stb/stb_truetype.h"

class PActor;

struct PFont
{
	stbtt_fontinfo Info;
	uint8_t* Bitmap;
	stbtt_bakedchar CharacterData[FONT_CHAR_COUNT]; // ASCII 32..126
	SDL_Texture* Texture;
};

class PRenderer
{
	SDLContext* mContext;
	FMatrix mMVP;
	SDL_Texture* mRenderTarget;

public:
	explicit PRenderer(SDLContext* InContext)
		: mContext(InContext), mRenderTarget(nullptr) {}

	bool Initialize();
	void PostInitialize() const;
	void Uninitialize() const;

	void OnResize(const FVector2& Size);

	SDL_Renderer* GetSDLRenderer()
	{
		return mContext->Renderer;
	}

	SDL_Window* GetRenderWindow() const
	{
		return SDL_GetRenderWindow(mContext->Renderer);
	}

	float GetScreenWidth() const;
	float GetScreenHeight() const;
	FVector2 GetScreenSize() const;
	FRect GetScreenRect() const;
	FRect GetViewport() const;
	FVector2 GetMousePosition() const;
	FVector2 GetMouseWorldPosition() const;
	bool GetMouseLeftDown() const;

	/* Fonts */

	void LoadFont(const std::string& Name) const;
	void UnloadFonts();
	float GetTextWidth(const std::string& Text, float FontSize = FONT_RENDER_SCALE) const;

	/* Math*/

	bool WorldToScreen(const FVector2& WorldPosition, FVector2* ScreenPosition) const;
	bool WorldToScreen(const FRect& WorldRect, FRect* ScreenRect) const;
	bool ScreenToWorld(const FVector2& ScreenPosition, FVector2* WorldPosition) const;

	PActor* GetActorUnderMouse() const;

	/* Drawing */

	bool Render() const;

	void SetRenderDrawBlendMode(SDL_BlendMode BlendMode) const;
	PColor GetDrawColor() const;
	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void SetDrawColor(const PColor& Color) const;
	void SetDrawAlpha(uint8_t Alpha) const;

	void SetClipRect(const FRect& ClipRect) const;
	void ReleaseClipRect() const;

	void DrawPoint(const FVector2& V, float Thickness = 0.0f) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect, float Thickness = 0.0f) const;
	void DrawFillRect(const FRect& Rect) const;
	void DrawPolygon(const std::vector<FVector2>& Vertices, const std::vector<int>& Indexes) const;
	void DrawGrid() const;
	float DrawText(const std::string& Text, const FVector2& Position, float FontSize) const;
	void DrawTexture(const PTexture* Texture, const FRect& Source, const FRect& Dest) const;

	void DrawPointAt(const FVector2& Position, float Thickness = 0.0f) const;
	void DrawLineAt(const FVector2& Start, const FVector2& End) const;
	void DrawRectAt(const FRect& Rect, float Thickness = 0.0f) const;
	void DrawFillRectAt(const FRect& Rect) const;
	void DrawTextAt(const std::string& Text, const FVector2& Position, float FontSize) const;
	void DrawTextureAt(const PTexture* Texture, const FRect& Source, const FRect& Dest) const;
	void DrawSpriteAt(const PTexture* Texture, const FRect& Dest,
	                  int32_t Index) const;
};

DECLARE_STATIC_GLOBAL_GETTER(Renderer)
