// ReSharper disable CppDFAUnreachableCode

#include "Grid.h"

#include "Core/Logging.h"
#include "Engine/Engine.h"
#include "Engine/InputManager.h"

constexpr int32_t GridWidth = 20; // Example grid size
constexpr int32_t GridHeight = 18;
static int		  PaletteTownTileData[18][20] = {
	   { 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 },
	   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	   { 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

void PTile::Draw(const PRenderer* Renderer, const FVector2& Offset) const
{
	const FRect	   Source = { float(X * 16), float(Y * 16), 16,
							  16 }; // Assuming each tile is 16x16 pixels
	const FRect	   Dest = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };
	const FVector2 Position = GetPosition();

	Renderer->DrawTextureAt(Texture, Source, Dest, Position);

	// Debug drawing
	if (GetSettings()->bDebugDraw)
	{
		if (bWalkable)
		{
			Renderer->SetDrawColor(200, 200, 200, 128); // Light gray outline for walkable tiles
			Renderer->DrawRectAt(Dest, Position + Offset);
		}
		else
		{
			Renderer->SetDrawColor(255, 0, 0, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(Dest, Position + Offset);
		}
		Renderer->SetDrawColor(255, 255, 0, 255);
		Renderer->DrawPointAt(Position, 2.0f); // Draw a point at the tile's position
	}
}

PActor* PTile::GetActor() const
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

void PGrid::Start()
{
	mPriority = DP_BACKGROUND;

	bBlocking = false;
	mTexture = PTextureManager::Load("PalletTown.png");

	// Instantiate each tile in the grid
	for (int Row = 0; Row < GridHeight; ++Row)
	{
		for (int Col = 0; Col < GridWidth; ++Col)
		{
			auto Tile = &mTiles.emplace_back(Col, Row);
			Tile->bWalkable = !PaletteTownTileData[Row][Col];
			Tile->Texture = mTexture;
		}
	}

	// Bind input
	if (const auto Input = GetInputManager())
	{
		Input->KeyUp.AddRaw(this, &PGrid::OnKeyUp);
	}
	else
	{
		LogError("Unable to bind input for PGrid.");
	}
}

void PGrid::Draw(const PRenderer* Renderer) const
{
	for (const auto& Tile : mTiles)
	{
		Tile.Draw(Renderer, mPosition);
	}
}

FRect PGrid::GetLocalBounds() const
{
	return { 0, 0, GridWidth * HALF_TILE_SIZE, GridHeight * HALF_TILE_SIZE };
}

PTile* PGrid::GetTileAtPosition(const FVector2& Position)
{
	for (auto& Tile : mTiles)
	{
		const FVector2 TilePosition = Tile.GetPosition();
		if (Position.X >= TilePosition.X					 // Min X
			&& Position.X < TilePosition.X + HALF_TILE_SIZE	 // Max X
			&& Position.Y >= TilePosition.Y					 // Min Y
			&& Position.Y < TilePosition.Y + HALF_TILE_SIZE) // Max Y
		{
			// Return a pointer to the tile
			return &Tile;
		}
	}
	return nullptr; // No tile found at the given position
}

void PGrid::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case KB_G:
			bDebugDraw = !bDebugDraw;
			break;
		default:
			break;
	}
}