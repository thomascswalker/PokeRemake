#include "Frame.h"

#include "Core/GameConstants.h"

#define DEST(OffsetX, OffsetY) FRect(OffsetX + mFixedPosition.X, OffsetY + mFixedPosition.Y, SCREEN_TILE, SCREEN_TILE)

void PFrame::Initialize(uint32_t SizeX, uint32_t SizeY)
{
	mSprite = std::make_shared<PSprite>();
	mSprite->SetTexture(GTextureManager->Get("Interface"));

	// Store all static source/dest pairs for each tile component of this frame.

	// Top left
	mFrameTiles.emplace_back(UI_INDEX_FRAME_1, DEST(0, 0));

	// Top row
	for (int Index = 1; Index < SizeX; Index++)
	{
		mFrameTiles.emplace_back(UI_INDEX_FRAME_5, DEST(COORD(Index), 0));
	}

	// Top right
	mFrameTiles.emplace_back(UI_INDEX_FRAME_3, DEST(COORD(SizeX), 0));

	// Right side
	for (int Index = 1; Index < SizeY; Index++)
	{
		mFrameTiles.emplace_back(UI_INDEX_FRAME_6, DEST(COORD(SizeX), COORD(Index)));
	}

	// Bottom right
	mFrameTiles.emplace_back(UI_INDEX_FRAME_4, DEST(COORD(SizeX), COORD(SizeY)));

	// Bottom row
	for (int Index = 1; Index < SizeX; Index++)
	{
		mFrameTiles.emplace_back(UI_INDEX_FRAME_5, DEST(COORD(Index), COORD(SizeY)));
	}

	// Bottom left
	mFrameTiles.emplace_back(UI_INDEX_FRAME_2, DEST(0, COORD(SizeY)));

	// Left side
	for (int Index = 1; Index < SizeY; Index++)
	{
		mFrameTiles.emplace_back(UI_INDEX_FRAME_6, DEST(0, COORD(Index)));
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
	for (const auto& [Index, Dest] : mFrameTiles)
	{
		Renderer->DrawSprite(mSprite.get(), Dest, Index);
	}
}