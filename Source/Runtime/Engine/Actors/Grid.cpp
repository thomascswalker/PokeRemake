// ReSharper disable CppDFAUnreachableCode

#include "Grid.h"

#include "Core/Logging.h"
#include "Engine/Engine.h"
#include "Engine/InputManager.h"

constexpr int32_t GridSize = 40; // Example grid size

void PTile::Draw(const PRenderer* Renderer, const FVector2& Offset) const
{
	const FRect	   Rect = { 0, 0, TILE_SIZE, TILE_SIZE };
	const FVector2 Position = GetPosition();

	// Rainbow gradient along X/Y axes
	const auto Red = static_cast<float>(X) / static_cast<float>(GridSize) * 255.0f;
	const auto Green = static_cast<float>(Y) / static_cast<float>(GridSize) * 255.0f;
	const auto Blue = 128;

	Renderer->SetDrawColor(Red, Green, Blue, 255);
	Renderer->DrawRectAt(Rect, Position + Offset);

	Renderer->SetDrawColor(255, 255, 0, 255);
	Renderer->DrawPointAt(Position, 2.0f); // Draw a point at the tile's position
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
	// Instantiate each tile in the grid
	for (int Row = 0; Row < GridSize; ++Row)
	{
		for (int Col = 0; Col < GridSize; ++Col)
		{
			mTiles.emplace_back(Col, Row);
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

	bBlocking = false;
}

void PGrid::Draw(const PRenderer* Renderer) const
{
	if (GetSettings()->bDebugDraw)
	{
		for (const auto& Tile : mTiles)
		{
			Tile.Draw(Renderer, mPosition);
		}
	}
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