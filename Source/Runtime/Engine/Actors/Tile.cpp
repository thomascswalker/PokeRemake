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
		FRect HoverRect = { WorldPosition.X, WorldPosition.Y, TILE_SIZE, TILE_SIZE };
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
	auto TilePosition = GetPosition();
	return Position.X >= TilePosition.X				   // Min X
		   && Position.X < TilePosition.X + TILE_SIZE  // Max X
		   && Position.Y >= TilePosition.Y			   // Min Y
		   && Position.Y < TilePosition.Y + TILE_SIZE; // Max Y
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

SBlock PTile::GetBlock() const
{
	SBlock Block;
	int	   CountX = Chunk->GetSizeX() / 2;
	int	   BlockX = (X / 2) * 2;
	int	   BlockY = (Y / 2) * 2;
	int	   BlockIndex = BlockY * CountX + BlockX;

	Block.Tiles[0] = Chunk->GetTile(BlockIndex);
	Block.Tiles[1] = Chunk->GetTile(BlockIndex + 1);
	Block.Tiles[2] = Chunk->GetTile(BlockIndex + CountX);
	Block.Tiles[3] = Chunk->GetTile(BlockIndex + CountX + 1);

	return Block;
}

PTile* PTile::GetAdjacent(int32_t InX, int32_t InY)
{
	if (!Chunk)
	{
		return nullptr;
	}

	return Chunk->GetTileAt(X + InX, Y + InY);
}

bool PTile::IsBlocking() const
{
	for (auto Index : Tileset->Blocking)
	{
		if (Index == TilesetIndex)
		{
			return true;
		}
	}
	return false;
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
