// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "Core/Logging.h"
#include "Engine/Engine.h"
#include "Engine/InputManager.h"

constexpr int32_t GridWidth = 20; // Example grid size
constexpr int32_t GridHeight = 18;
static int		  PaletteTownTileData[18][20] = {
	   { 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 0, 0, 1, 0 },
	   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 1, 1, 5, 1, 1, 0, 0, 0, 1, 1, 5, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 1, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

void STile::Draw(const PRenderer* Renderer, const FVector2& Offset) const
{
	const FRect	   Source = { float(X * 16), float(Y * 16), 16,
							  16 }; // Assuming each tile is 16x16 pixels
	const FRect	   Dest = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };
	const FVector2 Position = GetPosition();

	Renderer->DrawTextureAt(Texture, Source, Dest, Position);

	// Debug drawing
	if (GetSettings()->bDebugDraw)
	{
		switch (Type)
		{

			case TT_Normal:
				Renderer->SetDrawColor(200, 200, 200, 128); // Light gray outline for walkable tiles
				Renderer->DrawRectAt(Dest, Position + Offset);
				break;
			case TT_Obstacle:
				Renderer->SetDrawColor(255, 0, 0, 128);
				Renderer->DrawFillRectAt(Dest, Position + Offset);
				break;
			case TT_Water:
				Renderer->SetDrawColor(0, 0, 255, 128); // Red for non-walkable tiles
				Renderer->DrawFillRectAt(Dest, Position + Offset);
				break;
			case TT_Grass:
				Renderer->SetDrawColor(0, 255, 0, 128); // Red for non-walkable tiles
				Renderer->DrawFillRectAt(Dest, Position + Offset);
				break;
			case TT_Cave:
				break;
			case TT_Portal:
				Renderer->SetDrawColor(255, 0, 255, 128); // Red for non-walkable tiles
				Renderer->DrawFillRectAt(Dest, Position + Offset);
		}
		Renderer->SetDrawColor(255, 255, 0, 255);
		Renderer->DrawPointAt(Position, 2.0f); // Draw a point at the tile's position
	}
}

PActor* STile::GetActor() const
{
	for (const auto& Actor : GetWorld()->GetActors())
	{
		if (Actor->GetPosition() == GetPosition())
		{
			return Actor; // Return the actor if its position matches the tile's position
		}
	}
	return nullptr;
}

void PChunk::Start()
{
	mPriority = DP_BACKGROUND;

	bBlocking = false;
	auto T = PTextureManager::Load("PalletTown.png");
	mSprite.SetTexture(T);

	// Instantiate each tile in the grid
	for (int Row = 0; Row < GridHeight; ++Row)
	{
		for (int Col = 0; Col < GridWidth; ++Col)
		{
			auto Tile = &mTiles.emplace_back(Col, Row);
			Tile->Type = static_cast<ETileType>(PaletteTownTileData[Row][Col]);
			Tile->Texture = T;
		}
	}
}

void PChunk::Draw(const PRenderer* Renderer) const
{
	for (const auto& Tile : mTiles)
	{
		Tile.Draw(Renderer, mPosition);
	}
}

FRect PChunk::GetLocalBounds() const
{
	return { 0, 0, GridWidth * HALF_TILE_SIZE, GridHeight * HALF_TILE_SIZE };
}

STile* PChunk::GetTileAtPosition(const FVector2& Position)
{
	for (auto& Tile : mTiles)
	{
		if (Tile.Contains(Position))
		{
			// Return a pointer to the tile
			return &Tile;
		}
	}
	return nullptr; // No tile found at the given position
}
