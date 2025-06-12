#pragma once

#include "Application/Context.h"
#include "Core/Matrix.h"
#include "Core/Rect.h"
#include "Engine/Texture.h"
#include "stb/stb_truetype.h"
#include <vector>

class PActor;

struct PFont
{
	stbtt_fontinfo	Info;
	uint8_t*		Bitmap;
	stbtt_bakedchar CharacterData[FONT_CHAR_COUNT]; // ASCII 32..126
	SDL_Texture*	Texture;
};

class PRenderer
{
	SDLContext* mContext;
	FMatrix		mMVP;

public:
	explicit PRenderer(SDLContext* InContext) : mContext(InContext) {}

	bool Initialize() const;
	void PostInitialize() const;
	void Uninitialize() const;

	void LoadFont(const std::string& Name) const;
	void UnloadFonts();

	void Render() const;
	bool WorldToScreen(const FVector2& Position, FVector2* ScreenPosition) const;

	/* Drawing */

	void SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const;
	void DrawPoint(const FVector2& V, float Thickness = 0.0f) const;
	void DrawLine(float X1, float Y1, float X2, float Y2) const;
	void DrawRect(const FRect& Rect) const;
	void DrawFillRect(const FRect& Rect) const;
	void DrawPolygon(const std::vector<FVector2>& Vertices, const std::vector<int>& Indexes) const;
	void DrawGrid() const;
	void DrawText(const std::string& Text, const FVector2& Position, float FontSize) const;

	void DrawPointAt(const FVector2& Position, float Thickness = 0.0f) const;
	void DrawLineAt(const FVector2& Start, const FVector2& End) const;
	void DrawRectAt(const FRect& Rect, const FVector2& Position) const;
	void DrawFillRectAt(const FRect& Rect, const FVector2& Position) const;
	void DrawTextureAt(const PTexture* Texture, const FRect& Source, const FRect& Dest,
					   const FVector2& Position) const;
	void DrawSpriteAt(const PTexture* Texture, const FRect& Rect, const FVector2& Position,
					  int32_t Index) const;

	float GetTextWidth(const std::string& Text) const;

	SDL_Window* GetRenderWindow() const { return SDL_GetRenderWindow(mContext->Renderer); }

	float	 GetScreenWidth() const;
	float	 GetScreenHeight() const;
	FVector2 GetScreenSize() const;
	FRect	 GetViewport() const;
	FVector2 GetMousePosition() const;
	bool	 GetMouseLeftDown() const;
	PActor*	 GetActorAtUnderMouse() const;
};

DECLARE_STATIC_GLOBAL_GETTER(Renderer)
