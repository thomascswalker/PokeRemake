#include "Frame.h"

#include "Core/GameConstants.h"

static const FVector2 GSpriteSize(8, 8);
static const FVector2 GTopLeftIndex(0, 0);
static const FVector2 GBottomLeftIndex(8, 0);
static const FVector2 GTopRightIndex(16, 0);
static const FVector2 GBottomRightIndex(24, 0);
static const FVector2 GTopBottomIndex(32, 0);
static const FVector2 GLeftRightIndex(32, 8);

#define SOURCE(Position)	   FRect(Position, GSpriteSize)
#define DEST(OffsetX, OffsetY) FRect(OffsetX + mFixedPosition.X, OffsetY + mFixedPosition.Y, SCREEN_TILE, SCREEN_TILE)

void PFrame::Initialize(uint32_t SizeX, uint32_t SizeY)
{
	mTexture = GTextureManager->Get("Interface");

	// Store all static source/dest pairs for each tile component of this frame.

	// Top left
	mFrameTiles.emplace_back(SOURCE(GTopLeftIndex), DEST(0, 0));

	// Top row
	for (int Index = 1; Index < SizeX; Index++)
	{
		mFrameTiles.emplace_back(SOURCE(GTopBottomIndex), DEST(COORD(Index), 0));
	}

	// Top right
	mFrameTiles.emplace_back(SOURCE(GTopRightIndex), DEST(COORD(SizeX), 0));

	// Right side
	for (int Index = 1; Index < SizeY; Index++)
	{
		mFrameTiles.emplace_back(SOURCE(GLeftRightIndex), DEST(COORD(SizeX), COORD(Index)));
	}

	// Bottom right
	mFrameTiles.emplace_back(SOURCE(GBottomRightIndex), DEST(COORD(SizeX), COORD(SizeY)));

	// Bottom row
	for (int Index = 1; Index < SizeX; Index++)
	{
		mFrameTiles.emplace_back(SOURCE(GTopBottomIndex), DEST(COORD(Index), COORD(SizeY)));
	}

	// Bottom left
	mFrameTiles.emplace_back(SOURCE(GBottomLeftIndex), DEST(0, COORD(SizeY)));

	// Left side
	for (int Index = 1; Index < SizeY; Index++)
	{
		mFrameTiles.emplace_back(SOURCE(GLeftRightIndex), DEST(0, COORD(Index)));
	}
}

PFrame::PFrame(const FRect& Geometry)
{
	mFixedPosition.X = COORD(Geometry.X);
	mFixedPosition.Y = COORD(Geometry.Y);
	mFixedSize.X = COORD(Geometry.W);
	mFixedSize.Y = COORD(Geometry.H);

	// Negate 1 from width/height because the first index is one of the corners
	Initialize(Geometry.W - 1, Geometry.H - 1);
}

void PFrame::Draw(const PRenderer* Renderer) const
{
	// Fill the shape with white
	Renderer->SetDrawColor(PColor::White);
	Renderer->DrawFillRect(FRect(mFixedPosition, mFixedSize));

	// Draw each border tile
	for (const auto& [Source, Dest] : mFrameTiles)
	{
		Renderer->DrawTexture(mTexture, Source, Dest);
	}
}