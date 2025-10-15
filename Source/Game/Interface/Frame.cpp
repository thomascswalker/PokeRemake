#include "Frame.h"

#include "Core/GameConstants.h"

#define DEST(OffsetX, OffsetY) FVector2(OffsetX + mFixedPosition.X, OffsetY + mFixedPosition.Y)

void PFrame::Initialize(const FRect& Geometry)
{
	mSprite = std::make_shared<PSprite>();
	mSprite->SetTexture(GTextureManager->Get("Interface"));

	// Store all static source/dest pairs for each tile component of this frame.

	int Width = Geometry.W - 1;
	int Height = Geometry.H - 1;

	int MinX = Geometry.X;
	int MinY = Geometry.Y;
	int MaxX = Geometry.X + Width;
	int MaxY = Geometry.Y + Height;

	// Top left

	mFrameTiles.emplace_back(MinX, MinY, UI_INDEX_FRAME_TOPLEFT);

	// Top row
	for (int Index = MinX + 1; Index < MaxX; Index++)
	{
		mFrameTiles.emplace_back(Index, MinY, UI_INDEX_FRAME_HORIZONTAL);
	}

	// Top right
	mFrameTiles.emplace_back(MaxX, MinY, UI_INDEX_FRAME_TOPRIGHT);

	// Right side
	for (int Index = MinY + 1; Index < MaxY; Index++)
	{
		mFrameTiles.emplace_back(MaxX, Index, UI_INDEX_FRAME_VERTICAL);
	}

	// Bottom right
	mFrameTiles.emplace_back(MaxX, MaxY, UI_INDEX_FRAME_BOTTOMRIGHT);

	// Bottom row
	for (int Index = MinX + 1; Index < MaxX; Index++)
	{
		mFrameTiles.emplace_back(Index, MaxY, UI_INDEX_FRAME_HORIZONTAL);
	}

	// Bottom left
	mFrameTiles.emplace_back(MinX, MaxY, UI_INDEX_FRAME_BOTTOMLEFT);

	// Left side
	for (int Index = MinY + 1; Index < MaxY; Index++)
	{
		mFrameTiles.emplace_back(MinX, Index, UI_INDEX_FRAME_VERTICAL);
	}
}

PFrame::PFrame(const FRect& Geometry)
{
	mFixedPosition.X = COORD(Geometry.X);
	mFixedPosition.Y = COORD(Geometry.Y);
	mFixedSize.X = COORD(Geometry.W);
	mFixedSize.Y = COORD(Geometry.H);

	// Negate 1 from width/height because the first index is one of the corners
	Initialize(Geometry);
}

void PFrame::Draw(const PRenderer* Renderer) const
{
	// Fill the shape with white
	Renderer->SetDrawColor(PColor::White);
	Renderer->DrawFillRect(FRect(mFixedPosition, mFixedSize));

	// Draw each border tile
	for (const auto& [Dest, Index] : mFrameTiles)
	{
		Renderer->DrawSprite(mSprite.get(), Dest, Index);
	}
}