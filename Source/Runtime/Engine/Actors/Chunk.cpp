// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "Engine/Engine.h"
#include "Engine/InputManager.h"

void STile::Draw(const PRenderer* Renderer, const FVector2& Offset) const
{
	const FRect	   Source = { float(X * 16), float(Y * 16), 16,
							  16 }; // Assuming each tile is 16x16 pixels
	const FRect	   Dest = { Offset.X, Offset.Y, HALF_TILE_SIZE, HALF_TILE_SIZE };
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
	auto T = PTextureManager::Load(mTextureName);
	mSprite.SetTexture(T);

	mPosition = FVector2(mGeometry.X * HALF_TILE_SIZE, mGeometry.Y * HALF_TILE_SIZE);

	// Instantiate each tile in the grid
	for (int X = 0; X < mGeometry.W; X++)
	{
		for (int Y = 0; Y < mGeometry.H; Y++)
		{
			const auto Tile = &mTiles.emplace_back(X, Y);
			Tile->Type = static_cast<ETileType>(mData[Y][X]); // Access [Row][Column]
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
	return { mPosition.X, mPosition.Y, mGeometry.W * HALF_TILE_SIZE, mGeometry.H * HALF_TILE_SIZE };
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
