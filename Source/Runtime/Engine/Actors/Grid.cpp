#include "Grid.h"

#include "Engine/Engine.h"
#include "Engine/InputManager.h"

#include <cmath>
constexpr int32_t GridSize = 40; // Example grid size

void PGrid::Start()
{
	// Instantiate each tile in the grid
	for (int Row = 0; Row < GridSize; ++Row)
	{
		for (int Col = 0; Col < GridSize; ++Col)
		{
			mTiles.emplace_back(Col, Row);
		}
	}

	mPosition.X = -(GridSize * TILE_SIZE * 0.5f);
	mPosition.Y = mPosition.X - HALF_TILE_SIZE;
}

void PGrid::Draw(const PRenderer* Renderer) const
{
	for (const auto& Tile : mTiles)
	{
		FRect	 Rect = { 0, 0, TILE_SIZE, TILE_SIZE };
		FVector2 Position = { Tile.X * TILE_SIZE, Tile.Y * TILE_SIZE };

		auto Red = (static_cast<float>(Tile.X) / static_cast<float>(GridSize)) * 255.0f;
		auto Green = (static_cast<float>(Tile.Y) / static_cast<float>(GridSize)) * 255.0f;
		auto Blue = 128; // Fixed blue value for simplicity

		Renderer->SetDrawColor(Red, Green, Blue, 255);
		Renderer->DrawFillRectAt(Rect, Position + mPosition);
	}
}
