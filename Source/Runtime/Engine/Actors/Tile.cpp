#include "Tile.h"

#include "../World.h"
#include "Chunk.h"
#include "Core/Settings.h"

FVector2 STile::GetLocalPosition() const
{
	return FVector2(X * TILE_SIZE, Y * TILE_SIZE);
}

FVector2 STile::GetWorldPosition() const
{
	return GetLocalPosition() + Chunk->GetPosition();
}

void STile::Draw(const PRenderer* Renderer) const
{
	// Source rectangle to extract the tile from the texture
	// Assuming each tile is 16x16 pixels
	const FRect Source = { static_cast<float>(X * 16), static_cast<float>(Y * 16), 16, 16 };

	// World position of this tile
	const FVector2 WorldPosition = GetWorldPosition();

	// Screenspace destination rectangle
	const FRect Dest = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };

	Renderer->DrawTextureAt(Texture, Source, Dest, WorldPosition);

	if (!GetSettings()->bDebugDraw)
	{
		return;
	}

	// Debug drawing
	switch (Type)
	{
		case TT_Normal:
			break;
		case TT_Obstacle:
			Renderer->SetDrawColor(255, 0, 0, 128);
			Renderer->DrawFillRectAt(Dest, WorldPosition);
			break;
		case TT_Water:
			Renderer->SetDrawColor(0, 0, 255, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(Dest, WorldPosition);
			break;
		case TT_Grass:
		case TT_Cave:
			Renderer->SetDrawColor(0, 255, 0, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(Dest, WorldPosition);
			break;
		case TT_Portal:
			Renderer->SetDrawColor(255, 0, 255, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(Dest, WorldPosition);
	}

	Renderer->SetDrawColor(200, 200, 200, 128); // Light gray outline for walkable tiles
	Renderer->DrawRectAt(Dest, WorldPosition);
}

PActor* STile::GetActor() const
{
	for (const auto& Actor : GetWorld()->GetActors())
	{
		if (Actor->GetPosition() == GetWorldPosition())
		{
			return Actor; // Return the actor if its position matches the tile's world position
		}
	}
	return nullptr;
}

bool STile::IsWalkable() const
{
	return Type != TT_Obstacle && Type != TT_Water;
}

bool STile::Contains(const FVector2& Position) const
{
	auto TilePosition = GetWorldPosition();
	return Position.X >= TilePosition.X						// Min X
		   && Position.X < TilePosition.X + HALF_TILE_SIZE	// Max X
		   && Position.Y >= TilePosition.Y					// Min Y
		   && Position.Y < TilePosition.Y + HALF_TILE_SIZE; // Max Y
}
