#include "Renderer.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include "Core/Files.h"
#include "Core/Logging.h"
#include "Engine/Game.h"
#include "Engine/Texture.h"
#include "Engine/World.h"

std::string	 gDefaultFont = "Roboto-Regular"; // Default font name
static PFont gCurrentFont;

bool PRenderer::Initialize() const
{
	LoadFont(gDefaultFont);
	return true;
}

void PRenderer::PostInitialize() const
{
	PTextureManager::LoadSDL(mContext->Renderer);
}

void PRenderer::Uninitialize() const
{
	PTextureManager::UnloadSDL();
}

void PRenderer::LoadFont(const std::string& Name) const
{
	const auto FontFileName = Files::FindFile(std::format("{}.ttf", Name.c_str()));
	if (FontFileName.empty())
	{
		LogError("Failed to find font file: {}", Name.c_str());
		return;
	}
	FILE* FontFile = fopen(FontFileName.c_str(), "rb");
	fseek(FontFile, 0, SEEK_END);
	const int64_t Size = ftell(FontFile);
	fseek(FontFile, 0, SEEK_SET);

	const auto FontBuffer = static_cast<uint8_t*>(malloc(Size * sizeof(uint8_t)));

	fread(FontBuffer, Size, 1, FontFile);
	fclose(FontFile);

	gCurrentFont.Info = stbtt_fontinfo();
	if (!stbtt_InitFont(&gCurrentFont.Info, FontBuffer, 0))
	{
		LogError("Failed to initialize font: {}", FontFileName.c_str());
		free(FontBuffer);
		return;
	}
	LogDebug("Loaded font: {}", FontFileName.c_str());

	const size_t TempSize = FONT_ATLAS_SIZE * FONT_ATLAS_SIZE * sizeof(uint8_t);
	const auto	 Bitmap = static_cast<uint8_t*>(malloc(TempSize));
	stbtt_BakeFontBitmap(FontBuffer, 0, FONT_ATLAS_BAKE_SCALE, Bitmap, FONT_ATLAS_SIZE,
						 FONT_ATLAS_SIZE, FONT_CHAR_START, FONT_CHAR_COUNT,
						 gCurrentFont.CharacterData);

	gCurrentFont.Bitmap = static_cast<uint8_t*>(malloc(TempSize * 4));

	// Expand the baked font atlas to RGBA format
	for (int i = 0; i < FONT_ATLAS_SIZE * FONT_ATLAS_SIZE; i++)
	{
		gCurrentFont.Bitmap[i * 4] = Bitmap[i];
		gCurrentFont.Bitmap[i * 4 + 1] = Bitmap[i];
		gCurrentFont.Bitmap[i * 4 + 2] = Bitmap[i];
		gCurrentFont.Bitmap[i * 4 + 3] = Bitmap[i];
	}

	free(Bitmap);
	free(FontBuffer);
	gCurrentFont.Texture =
		SDL_CreateTexture(mContext->Renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC,
						  FONT_ATLAS_SIZE, FONT_ATLAS_SIZE);
	const SDL_Rect Rect(0, 0, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE);
	SDL_UpdateTexture(gCurrentFont.Texture, &Rect, gCurrentFont.Bitmap,
					  FONT_ATLAS_SIZE * sizeof(uint32_t));
}

void PRenderer::UnloadFonts() {}

void PRenderer::Render() const
{
	SDL_SetRenderDrawColor(mContext->Renderer, 38, 38, 38, 255);
	SDL_RenderClear(mContext->Renderer);

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		for (const IDrawable* Drawable : World->GetDrawables(DP_BACKGROUND))
		{
			Drawable->Draw(this);
		}
		for (const IDrawable* Drawable : World->GetDrawables(DP_FOREGROUND))
		{
			Drawable->Draw(this);
		}

		if (const auto Root = World->GetRootWidget())
		{
			SWidgetEvent Event;
			Event.MousePosition = GetMousePosition();
			Event.bMouseDown = GetMouseLeftDown();
			Root->ProcessEvents(&Event);

			// Recursively construct the layout of all widgets
			Root->LayoutChildren();

			// Recursively draw all widgets
			Root->Draw(this);
		}
	}

	SDL_RenderPresent(mContext->Renderer);
}

bool PRenderer::WorldToScreen(const FVector2& Position, FVector2* ScreenPosition) const
{
	const auto ScreenSize = GetScreenSize();

	if (const auto CameraView = GetCameraView())
	{
		const auto ViewPosition = CameraView->GetPosition();
		const auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
		const auto Offset = (Position - ViewPosition2D) * CameraView->GetZoom();

		*ScreenPosition = (Offset + ScreenSize) * 0.5f;
		return true;
	}
	return false;
}

void PRenderer::SetDrawColor(uint8_t R, uint8_t G, uint8_t B, uint8_t A) const
{
	SDL_SetRenderDrawColor(mContext->Renderer, R, G, B, A);
	if (A < 255) // Is there transparency?
	{
		SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_BLEND);
	}
	else
	{
		SDL_SetRenderDrawBlendMode(mContext->Renderer, SDL_BLENDMODE_NONE);
	}
}

void PRenderer::DrawPoint(const FVector2& V, float Thickness) const
{
	if (Thickness > 0.0f)
	{
		const SDL_FRect R = { V.X - Thickness / 2.0f, V.Y - Thickness / 2.0f, Thickness,
							  Thickness };
		SDL_RenderFillRect(mContext->Renderer, &R);
	}
	else
	{
		SDL_RenderPoint(mContext->Renderer, V.X, V.Y);
	}
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

void PRenderer::DrawPolygon(const std::vector<FVector2>& Vertices,
							const std::vector<int32_t>&	 Indexes) const
{
	float R, G, B, A;
	SDL_GetRenderDrawColorFloat(mContext->Renderer, &R, &G, &B, &A);
	std::vector<SDL_Vertex> SDLVertices;
	for (size_t i = 0; i < Vertices.size(); ++i)
	{
		SDL_Vertex V;
		V.position.x = Vertices[i].X;
		V.position.y = Vertices[i].Y;
		V.color = { R, G, B, A };
		SDLVertices.emplace_back(V);
	}
	SDL_RenderGeometry(mContext->Renderer, nullptr, SDLVertices.data(),
					   static_cast<int>(SDLVertices.size()), Indexes.data(),
					   static_cast<int>(Indexes.size()));
}

void PRenderer::DrawGrid() const
{
	SetDrawColor(100, 100, 100, 255);

	const float ScreenWidth = GetScreenWidth();
	const float ScreenHeight = GetScreenHeight();

	float X0, X1, Y0, Y1;

	// Draw vertical lines
	for (int32_t Index = 0; Index <= TILE_COLUMNS; Index++)
	{
		X0 = Index * HALF_TILE_SIZE;
		X1 = X0;
		Y0 = 0;
		Y1 = ScreenHeight;
		DrawLine(X0, Y0, X1, Y1);
	}

	// Draw horizontal lines
	for (int32_t Index = 0; Index <= TILE_COLUMNS; Index++)
	{
		X0 = 0;
		X1 = ScreenWidth;
		Y0 = Index * HALF_TILE_SIZE;
		Y1 = Y0;
		DrawLine(X0, Y0, X1, Y1);
	}
}

void PRenderer::DrawText(const std::string& Text, const FVector2& Position, float FontSize) const
{
	// Aspect ratio of the pixel height we render at to the pixel height we baked the
	// font atlas at.
	const float Aspect = FontSize / FONT_ATLAS_BAKE_SCALE;
	const float Width = GetTextWidth(Text);

	float		X = Position.X - Width / 2.0f;
	const float Y = Position.Y + FONT_RENDER_SCALE / 4.0f;
	for (const auto& C : Text)
	{
		const auto Info = &gCurrentFont.CharacterData[C - FONT_CHAR_START];
		SDL_FRect  Source(Info->x0, Info->y0, Info->x1 - Info->x0, Info->y1 - Info->y0);
		SDL_FRect  Dest(X + Info->xoff * Aspect, Y + Info->yoff * Aspect,
						(Info->x1 - Info->x0) * Aspect, (Info->y1 - Info->y0) * Aspect);
		SDL_RenderTexture(mContext->Renderer, gCurrentFont.Texture, &Source, &Dest);
		X += Info->xadvance * Aspect;
	}
}

void PRenderer::DrawPointAt(const FVector2& Position, float Thickness) const
{
	FVector2 ScreenPosition;
	WorldToScreen(Position, &ScreenPosition);
	DrawPoint(ScreenPosition, Thickness);
}

void PRenderer::DrawLineAt(const FVector2& Start, const FVector2& End) const
{
	FVector2 ScreenStart;
	FVector2 ScreenEnd;
	if (WorldToScreen(Start, &ScreenStart) && WorldToScreen(End, &ScreenEnd))
	{
		SDL_RenderLine(mContext->Renderer, ScreenStart.X, ScreenStart.Y, ScreenEnd.X, ScreenEnd.Y);
	}
}

void PRenderer::DrawRectAt(const FRect& Rect, const FVector2& Position) const
{
	FVector2 ScreenPosition;
	WorldToScreen(Position, &ScreenPosition);
	const auto CameraView = GetCameraView();
	DrawRect({ ScreenPosition.X, ScreenPosition.Y, Rect.W * CameraView->GetZoom(),
			   Rect.H * CameraView->GetZoom() });
}

void PRenderer::DrawFillRectAt(const FRect& Rect, const FVector2& Position) const
{
	FVector2 ScreenPosition;
	WorldToScreen(Position, &ScreenPosition);
	const auto CameraView = GetCameraView();
	DrawFillRect({ ScreenPosition.X, ScreenPosition.Y, Rect.W * CameraView->GetZoom(),
				   Rect.H * CameraView->GetZoom() });
}

void PRenderer::DrawTextureAt(const PTexture* Texture, const FRect& Source, const FRect& Dest,
							  const FVector2& Position) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	FVector2 ScreenPosition;
	WorldToScreen(Position, &ScreenPosition);
	const auto		Min = Dest.Min() + ScreenPosition;
	const auto		Max = Dest.Max() + ScreenPosition;
	const auto		CameraView = GetCameraView();
	const SDL_FRect Source2 = { Source.X, Source.Y, Source.W, Source.H };
	const SDL_FRect Dest2 = { Min.X, Min.Y, (Max.X - Min.X) * CameraView->GetZoom(),
							  (Max.Y - Min.Y) * CameraView->GetZoom() };

	SDL_RenderTexture(mContext->Renderer, Tex, &Source2, &Dest2);
}
void PRenderer::DrawSpriteAt(const PTexture* Texture, const FRect& Rect, const FVector2& Position,
							 int32_t Index) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	const auto ViewPosition = GetCameraView()->GetPosition();
	const auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);
	const auto Offset = Position - ViewPosition2D;
	const auto ScreenSize = GetScreenSize();

	auto Min = Rect.Min() + Offset;
	auto Max = Rect.Max() + Offset;

	// Camera position
	Min = (Min + ScreenSize) * 0.5f;
	Max = (Max + ScreenSize) * 0.5f;

	const float		SourceOffset = Index * SPRITE_WIDTH; // Assuming each sprite is 16x16 pixels
	const SDL_FRect Source = { SourceOffset, 0, SPRITE_WIDTH, SPRITE_WIDTH };
	const SDL_FRect Dest = { Min.X, Min.Y, Max.X - Min.X, Max.Y - Min.Y };

	SDL_RenderTexture(mContext->Renderer, Tex, &Source, &Dest);
}

float PRenderer::GetTextWidth(const std::string& Text) const
{
	constexpr float Aspect = FONT_RENDER_SCALE / FONT_ATLAS_BAKE_SCALE;
	float			Width = 0;
	for (const auto& C : Text)
	{
		const auto Info = &gCurrentFont.CharacterData[C - FONT_CHAR_START];
		Width += Info->xadvance * Aspect;
	}
	return Width;
}

float PRenderer::GetScreenWidth() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Width;
}

float PRenderer::GetScreenHeight() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return Height;
}

FVector2 PRenderer::GetScreenSize() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return { static_cast<float>(Width), static_cast<float>(Height) };
}

FRect PRenderer::GetViewport() const
{
	int32_t Width, Height;
	SDL_GetWindowSize(GetRenderWindow(), &Width, &Height);
	return { 0, 0, static_cast<float>(Width), static_cast<float>(Height) };
}

FVector2 PRenderer::GetMousePosition() const
{
	float X, Y;
	SDL_GetMouseState(&X, &Y);
	return { X, Y };
}

bool PRenderer::GetMouseLeftDown() const
{
	return SDL_GetMouseState(nullptr, nullptr) == 1;
}

PActor* PRenderer::GetActorUnderMouse() const
{
	auto W = GetWorld();
	for (const auto& Actor : W->GetActors())
	{
		if (Actor->IsMouseOver())
		{
			return Actor;
		}
	}
	return nullptr;
}
