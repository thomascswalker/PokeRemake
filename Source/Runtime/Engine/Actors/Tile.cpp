#include "Tile.h"

#include "../../../Editor/EditorGame.h"
#include "../Settings.h"
#include "../World.h"
#include "Chunk.h"

void PTile::Draw(const PRenderer* Renderer) const
{
	// World position of this tile.
	const FVector2 WorldPosition = GetWorldPosition();
	FRect		   DestRect = {
		 WorldPosition, { TILE_SIZE, TILE_SIZE }
	};

	// Draw the debug color indicating the texture is invalid.
	if (!Tileset || !Tileset->Texture)
	{
		Renderer->SetDrawColor(PColor::UIDebug1);
		Renderer->DrawFillRectAt(DestRect);
		return;
	}

	float TilesetX = TilesetIndex % Tileset->Width;
	float TilesetY = TilesetIndex / Tileset->Width;
	FRect SourceRect = { TilesetX * gTilesetItemSize, TilesetY * gTilesetItemSize, gTilesetItemSize, gTilesetItemSize };
	Renderer->DrawTextureAt(Tileset->Texture, SourceRect, DestRect);
}

void PTile::DebugDraw(const PRenderer* Renderer) const
{
	const FVector2 WorldPosition = GetWorldPosition();
	FRect		   DestRect = {
		 WorldPosition, { TILE_SIZE, TILE_SIZE }
	};
	if (IsBlocking())
	{
		Renderer->SetDrawColor(255, 0, 0, 128);
		Renderer->DrawFillRectAt(DestRect);
	}

	Renderer->SetDrawColor(200, 200, 200, 128); // Light gray outline for walkable tiles
	Renderer->DrawRectAt(DestRect);

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Tile) && mMouseOver)
	{
		auto G = GetEditorGame();
		auto Item = G->GetCurrentTilesetItem();
		if (!Item)
		{
			return;
		}
		auto BrushSize = G->GetBrushSize();
		Renderer->SetDrawColor(255, 0, 0, 255);
		FRect HoverRect = { WorldPosition.X, WorldPosition.Y, TILE_SIZE * BrushSize, TILE_SIZE * BrushSize };
		Renderer->DrawRectAt(HoverRect);
	}
#endif
}

PActor* PTile::GetActor() const
{
	for (const auto& Actor : GetWorld()->GetActors())
	{
		if (Actor->GetPosition() == GetPosition())
		{
			return Actor; // Return the actor if its position matches the tile's world position
		}
	}
	return nullptr;
}

bool PTile::IsWalkable() const
{
	return !IsBlocking();
}

bool PTile::Contains(const FVector2& Position) const
{
	auto BlockPosition = GetPosition();
	return Position.X >= BlockPosition.X				 // Min X
		   && Position.X < BlockPosition.X + BLOCK_SIZE	 // Max X
		   && Position.Y >= BlockPosition.Y				 // Min Y
		   && Position.Y < BlockPosition.Y + BLOCK_SIZE; // Max Y
}

FVector2 PTile::GetPosition() const
{
	if (Chunk)
	{
		auto Position = Chunk->GetPosition();
		Position.X += mPosition.X;
		Position.Y += mPosition.Y;
		return Position;
	}
	return mPosition;
}

PTile* PTile::GetAdjacent(int32_t InX, int32_t InY)
{
	if (!Chunk)
	{
		return nullptr;
	}

	return Chunk->GetBlockAt(X + InX, Y + InY);
}

json PTile::Serialize() const
{
	json Result;
	Result["Name"] = GetInternalName();
	Result["Class"] = GetClassName();
	Result["Position"] = { X, Y };
	Result["Index"] = TilesetIndex;
	return Result;
}

void PTile::Deserialize(const json& Data)
{
	PActor::Deserialize(Data);
}
