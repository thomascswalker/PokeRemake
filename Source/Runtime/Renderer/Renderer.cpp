#include "Renderer.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "Core/Files.h"
#include "Core/Logging.h"
#include "Core/Macros.h"
#include "Engine/Game.h"
#include "Engine/Texture.h"
#include "Engine/World.h"
#include "stb/stb_truetype.h"

std::string	 gDefaultFont = FONT_NAME; // Default font name
static PFont gCurrentFont;

constexpr auto gTextureScaleMode = SDL_SCALEMODE_NEAREST;
constexpr auto gTextureAddressMode = SDL_TEXTURE_ADDRESS_WRAP;

float PRenderer::DrawTextInternal(const std::string& Text, const FVector2& Position, float FontSize) const
{
	// uint8_t R, G, B, A;
	auto Color = GetDrawColor();
	SDL_SetTextureColorMod(gCurrentFont.Texture, Color.R, Color.G, Color.B);

	// Aspect ratio of the pixel height we render at to the pixel height we baked the
	// font atlas at.
	const float Aspect = FontSize / FONT_ATLAS_BAKE_SCALE;
	const float Width = GetTextWidth(Text, FontSize);

	float		X = Position.X;
	const float Y = Position.Y;
	for (const auto& C : Text)
	{
		const auto Info = &gCurrentFont.CharacterData[C - FONT_CHAR_START];
		SDL_FRect  Source(Info->x0, Info->y0, Info->x1 - Info->x0, Info->y1 - Info->y0);
		SDL_FRect  Dest(X + Info->xoff * Aspect, Y + Info->yoff * Aspect,
						(Info->x1 - Info->x0) * Aspect, (Info->y1 - Info->y0) * Aspect);
		SDL_RenderTexture(mContext->Renderer, gCurrentFont.Texture, &Source, &Dest);
		X += Info->xadvance * Aspect;
	}

	// Reset draw color mod
	SDL_SetTextureColorMod(gCurrentFont.Texture, 255, 255, 255);

	return Width;
}

bool PRenderer::Initialize()
{
	SDL_SetDefaultTextureScaleMode(mContext->Renderer, gTextureScaleMode);
	SDL_SetRenderTextureAddressMode(mContext->Renderer, gTextureAddressMode, gTextureAddressMode);

	mRenderTarget = SDL_GetRenderTarget(mContext->Renderer);
	LoadFont(gDefaultFont);

	return true;
}

void PRenderer::PostInitialize() const {}

void PRenderer::Uninitialize() const
{
	TextureManager::UnloadSDL();
}

void PRenderer::OnResize(const FVector2& Size)
{
	SDL_SetRenderLogicalPresentation(mContext->Renderer, Size.X, Size.Y, SDL_LOGICAL_PRESENTATION_DISABLED);
}

void PRenderer::LoadFont(const std::string& Name) const
{
	const std::string FontFileName = Files::FindFile(std::format("{}.ttf", Name.c_str()));
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
	gCurrentFont.Texture = SDL_CreateTexture(mContext->Renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC,
											 FONT_ATLAS_SIZE, FONT_ATLAS_SIZE);
	const SDL_Rect Rect(0, 0, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE);
	SDL_UpdateTexture(gCurrentFont.Texture, &Rect, gCurrentFont.Bitmap,
					  FONT_ATLAS_SIZE * sizeof(uint32_t));
}

void PRenderer::UnloadFonts() {}

bool PRenderer::Render() const
{
	SetDrawColor(PColor::UIBackground);
	SDL_RenderClear(mContext->Renderer);

	bool DebugDraw = GetSettings()->DebugDraw;

	if (!GetCameraView())
	{
		return false;
	}

	// Draw all renderables in the world
	if (const PWorld* World = GetWorld())
	{
		// Main draw
		auto Drawables = World->GetDrawables();
		for (IDrawable* Drawable : Drawables)
		{
			VALIDATE(Drawable->Draw(this),
					 dynamic_cast<PObject*>(Drawable)->GetInternalName().c_str());
		}

		// Debug draw
		if (DebugDraw)
		{
			for (IDrawable* Drawable : Drawables)
			{
				VALIDATE(Drawable->DebugDraw(this),
						 dynamic_cast<PObject*>(Drawable)->GetInternalName().c_str());
			}
		}

		if (const auto Root = World->GetHUD())
		{
			// Recursively draw all widgets, but not the root widget
			VALIDATE(Root->DrawChildren(this), "Failed to draw.");
		}

		for (auto W : GetWorld()->GetWidgets())
		{
			if (!W->GetFloating() || !W->GetVisible())
			{
				continue;
			}
			W->PreDraw(this);
			W->Draw(this);
			W->PostDraw(this);
			VALIDATE(W->DrawChildren(this), "Failed to draw.");
		}
	}

	SDL_RenderPresent(mContext->Renderer);
	return true;
}

void PRenderer::SetRenderDrawBlendMode(SDL_BlendMode BlendMode) const
{
	SDL_SetRenderDrawBlendMode(mContext->Renderer, BlendMode);
}

PColor PRenderer::GetDrawColor() const
{
	uint8_t R, G, B, A;
	SDL_GetRenderDrawColor(mContext->Renderer, &R, &G, &B, &A);
	return PColor{ R, G, B, A };
}

bool PRenderer::WorldToScreen(const FVector2& WorldPosition, FVector2* ScreenPosition) const
{
	const auto ScreenSize = GetScreenSize();

	const auto CameraView = GetGame()->GetCameraView();
	if (CameraView)
	{
		const auto ViewPosition = CameraView->GetPosition();
		const auto Offset = (WorldPosition - ViewPosition) * CameraView->GetZoom() * RENDER_SCALE;

		*ScreenPosition = (Offset + ScreenSize) * 0.5f;
		return true;
	}
	return false;
}

bool PRenderer::WorldToScreen(const FRect& WorldRect, FRect* ScreenRect) const
{
	FVector2 ScreenPosition, ScreenMax;
	WorldToScreen(WorldRect.GetPosition(), &ScreenPosition);
	WorldToScreen(WorldRect.Max(), &ScreenMax);
	ScreenRect->X = ScreenPosition.X;
	ScreenRect->Y = ScreenPosition.Y;
	ScreenRect->W = ScreenMax.X - ScreenPosition.X;
	ScreenRect->H = ScreenMax.Y - ScreenPosition.Y;
	return true;
}

bool PRenderer::ScreenToWorld(const FVector2& ScreenPosition, FVector2* WorldPosition) const
{
	const auto ScreenSize = GetScreenSize();

	if (const auto CameraView = GetCameraView())
	{
		auto Offset = (ScreenPosition / RENDER_SCALE / 0.5f) - ScreenSize;
		auto Position2D = Offset / CameraView->GetZoom();

		const auto ViewPosition = CameraView->GetPosition();
		const auto ViewPosition2D = FVector2(ViewPosition.X, ViewPosition.Y);

		*WorldPosition = Position2D + ViewPosition2D;
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

void PRenderer::SetDrawColor(const PColor& Color) const
{
	SetDrawColor(Color.R, Color.G, Color.B, Color.A);
}

void PRenderer::SetDrawAlpha(uint8_t Alpha) const
{
	uint8_t R, G, B, A;
	SDL_GetRenderDrawColor(mContext->Renderer, &R, &G, &B, &A);
	SDL_SetRenderDrawColor(mContext->Renderer, R, G, B, Alpha);
}

void PRenderer::SetClipRect(const FRect& ClipRect) const
{
	SDL_Rect Clip = ClipRect.ToSDL_Rect();
	SDL_SetRenderClipRect(mContext->Renderer, &Clip);
}

void PRenderer::ReleaseClipRect() const
{
	SDL_Rect Clip = { 0, 0, (int)GetScreenWidth(), (int)GetScreenHeight() };
	SDL_SetRenderClipRect(mContext->Renderer, &Clip);
}

void PRenderer::DrawPoint(const FVector2& V, float Thickness) const
{
	if (Thickness > 0.0f)
	{
		const SDL_FRect R = {
			V.X - Thickness / 2.0f, V.Y - Thickness / 2.0f, Thickness,
			Thickness
		};
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

void PRenderer::DrawLine(const FVector2& Start, const FVector2& End) const
{
	SDL_RenderLine(mContext->Renderer, Start.X, Start.Y, End.X, End.Y);
}

void PRenderer::DrawRect(const FRect& Rect, float Thickness) const
{
	if (!Rect.Overlaps(GetScreenRect()))
	{
		return;
	}
	SDL_FRect SRect;
	if (Thickness)
	{
		auto Outer = Rect.Expanded(Thickness);

		// Draw top edge
		auto TopRect = FRect{ Outer.X, Outer.Y, Outer.W, Thickness };
		DrawFillRect(TopRect);

		// Draw bottom edge
		auto BottomRect = FRect{ Outer.X, Outer.Y + Outer.H - Thickness, Outer.W, Thickness };
		DrawFillRect(BottomRect);

		// Draw left edge
		auto LeftRect = FRect{ Outer.X, Outer.Y, Thickness, Outer.H };
		DrawFillRect(LeftRect);

		// Draw right edge
		auto RightRect = FRect{ Outer.X + Outer.W - Thickness, Outer.Y, Thickness, Outer.H };
		DrawFillRect(RightRect);
	}
	else
	{
		SRect = { Rect.X, Rect.Y, Rect.W, Rect.H };
		SDL_RenderRect(mContext->Renderer, &SRect);
	}
}

void PRenderer::DrawFillRect(const FRect& Rect) const
{
	if (!Rect.Overlaps(GetScreenRect()))
	{
		return;
	}
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
		X0 = Index * TILE_SIZE;
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
		Y0 = Index * TILE_SIZE;
		Y1 = Y0;
		DrawLine(X0, Y0, X1, Y1);
	}
}

float PRenderer::DrawText(const std::string& Text, const FVector2& Position, float FontSize, bool Shadow) const
{
	FVector2 AlignedPosition = Position;

	// uint8_t R, G, B, A;
	if (Shadow)
	{
		auto Color = GetDrawColor();
		SetDrawColor(0, 0, 0, 255);
		DrawTextInternal(Text, AlignedPosition + FVector2{ -1, -1 }, FontSize);
		DrawTextInternal(Text, AlignedPosition + FVector2{ -1, 1 }, FontSize);
		DrawTextInternal(Text, AlignedPosition + FVector2{ 1, -1 }, FontSize);
		DrawTextInternal(Text, AlignedPosition + FVector2{ 1, 1 }, FontSize);
		SetDrawColor(Color);
	}
	return DrawTextInternal(Text, AlignedPosition, FontSize);
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

void PRenderer::DrawRectAt(const FRect& Rect, float Thickness) const
{
	FRect ScreenRect;
	WorldToScreen(Rect, &ScreenRect);
	DrawRect(ScreenRect, Thickness);
}

void PRenderer::DrawFillRectAt(const FRect& Rect) const
{
	FRect ScreenRect;
	WorldToScreen(Rect, &ScreenRect);
	DrawFillRect(ScreenRect);
}

void PRenderer::DrawTextAt(const std::string& Text, const FVector2& Position, float FontSize, bool Shadow) const
{
	FVector2 ScreenPosition;
	WorldToScreen(Position, &ScreenPosition);
	DrawText(Text, ScreenPosition, FontSize, Shadow);
}

void PRenderer::DrawTexture(const PTexture* Texture, const FRect& Source, const FRect& Dest) const
{
	if (!Texture)
	{
		return;
	}
	if (!Dest.Overlaps(GetScreenRect()))
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();
	auto		 Source2 = Source.ToSDL_FRect();
	auto		 Dest2 = Dest.ToSDL_FRect();
	SDL_RenderTexture(mContext->Renderer, Tex, &Source2, &Dest2);
}

void PRenderer::DrawTextureAt(const PTexture* Texture, const FRect& Source, const FRect& Dest) const
{
	if (!Texture)
	{
		return;
	}
	SDL_Texture* Tex = Texture->GetSDLTexture();

	FRect ScreenRect;
	WorldToScreen(Dest, &ScreenRect);

	const SDL_FRect SDLSource = Source.ToSDL_FRect();
	const SDL_FRect SDLDest = ScreenRect.ToSDL_FRect();

	SDL_RenderTexture(mContext->Renderer, Tex, &SDLSource, &SDLDest);
}

void PRenderer::DrawSpriteAt(const PTexture* Texture, const FRect& Dest,
							 int32_t Index) const
{
	if (!Texture)
	{
		return;
	}

	const float SourceOffset = Index * DEFAULT_SPRITE_WIDTH; // Assuming each sprite is 16x16 pixels
	const FRect Source = { SourceOffset, 0, DEFAULT_SPRITE_WIDTH, DEFAULT_SPRITE_WIDTH };
	DrawTextureAt(Texture, Source, Dest);
}

float PRenderer::GetTextWidth(const std::string& Text, const float FontSize) const
{
	const float Aspect = FontSize / FONT_ATLAS_BAKE_SCALE;
	float		Width = 0;
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
	SDL_GetWindowSizeInPixels(GetRenderWindow(), &Width, &Height);
	return Width;
}

float PRenderer::GetScreenHeight() const
{
	int32_t Width, Height;
	SDL_GetWindowSizeInPixels(GetRenderWindow(), &Width, &Height);
	return Height;
}

FVector2 PRenderer::GetScreenSize() const
{
	int32_t Width, Height;
	SDL_GetWindowSizeInPixels(GetRenderWindow(), &Width, &Height);
	return { static_cast<float>(Width), static_cast<float>(Height) };
}

FRect PRenderer::GetScreenRect() const
{
	return {
		{ 0, 0 },
		GetScreenSize()
	};
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

FVector2 PRenderer::GetMouseWorldPosition() const
{
	FVector2 MouseWorldPosition;
	GetRenderer()->ScreenToWorld(GetMousePosition(), &MouseWorldPosition);
	return MouseWorldPosition;
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
		if (Actor->mMouseOver)
		{
			return Actor;
		}
	}
	return nullptr;
}

TArray<PActor*> PRenderer::GetActorsUnderMouse() const
{
	TArray<PActor*> Actors;
	auto			W = GetWorld();
	for (const auto& Actor : W->GetActors())
	{
		if (Actor->mMouseOver)
		{
			Actors.Add(Actor);
		}
	}
	return Actors;
}
