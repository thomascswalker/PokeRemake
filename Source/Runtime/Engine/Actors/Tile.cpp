#include "Tile.h"

#include "../../../Editor/EditorGame.h"
#include "../World.h"
#include "Chunk.h"
#include "Core/Settings.h"

static FRect DoubleTileDest = { 0, 0, DOUBLE_TILE_SIZE, DOUBLE_TILE_SIZE };
static FRect FullTileDest = { 0, 0, TILE_SIZE, TILE_SIZE };
static FRect HalfTileDest = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };

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

void PTile::Draw(const PRenderer* Renderer) const
{
	// World position of this tile
	const FVector2 WorldPosition = GetPosition();

	auto Texture = Data.GetTexture();

	// Draw the debug color indicating the texture is invalid
	if (!Texture)
	{
		Renderer->SetDrawColor(PColor::UIDebug1);
		Renderer->DrawFillRectAt(DoubleTileDest, WorldPosition);
	}
	else
	{
		auto Dest = Data.IsSubIndexed() ? HalfTileDest : FullTileDest;
		if (!Data.IsSubIndexed())
		{
			Renderer->DrawTextureAt(Texture, Data.GetSourceRect(), Dest, WorldPosition);
		}
		else
		{
			for (int X = 0; X < 2; X++)
			{
				for (int Y = 0; Y < 2; Y++)
				{
					FVector2 LocalOffset = { X * TILE_SIZE, Y * TILE_SIZE };
					Renderer->DrawTextureAt(Texture, Data.GetSourceRect(Y * 2 + X), Dest, WorldPosition + LocalOffset);
				}
			}
		}
	}

	// If not in debug mode, return.
	if (!GetSettings()->mDebugDraw)
	{
		return;
	}

	// Debug drawing
	switch (Data.GetType())
	{
		case TT_Normal:
			break;
		case TT_Obstacle:
			Renderer->SetDrawColor(255, 0, 0, 128);
			Renderer->DrawFillRectAt(FullTileDest, WorldPosition);
			break;
		case TT_Water:
			Renderer->SetDrawColor(0, 0, 255, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(FullTileDest, WorldPosition);
			break;
		case TT_Grass:
		case TT_Cave:
			Renderer->SetDrawColor(0, 255, 0, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(FullTileDest, WorldPosition);
			break;
		case TT_Portal:
			Renderer->SetDrawColor(255, 0, 255, 128); // Red for non-walkable tiles
			Renderer->DrawFillRectAt(FullTileDest, WorldPosition);
	}

	Renderer->SetDrawColor(200, 200, 200, 128); // Light gray outline for walkable tiles
	Renderer->DrawRectAt(FullTileDest, WorldPosition);

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Tile) && (mMouseOver || mSelected))
	{
		if (!mMouseOver)
		{
			return;
		}
		auto G = GetEditorGame();
		auto Item = G->GetCurrentTilesetItem();
		if (!Item)
		{
			return;
		}
		Renderer->SetDrawColor(255, 0, 0, 255);
		if (Item->SizeType == TST_1X1)
		{
			auto  HoverPosition = GetQuadrant(Renderer->GetMouseWorldPosition()) * TILE_SIZE;
			FRect HoverRect = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };
			Renderer->DrawRectAt(HoverRect, WorldPosition + HoverPosition);
		}
		else
		{
			FRect HoverRect = { 0, 0, TILE_SIZE, TILE_SIZE };
			Renderer->DrawRectAt(HoverRect, WorldPosition);
		}
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
	auto Type = Data.GetType();
	return Type != TT_Obstacle && Type != TT_Water;
}

bool PTile::Contains(const FVector2& Position) const
{
	auto TilePosition = GetPosition();
	return Position.X >= TilePosition.X				   // Min X
		   && Position.X < TilePosition.X + TILE_SIZE  // Max X
		   && Position.Y >= TilePosition.Y			   // Min Y
		   && Position.Y < TilePosition.Y + TILE_SIZE; // Max Y
}

FVector2 PTile::GetQuadrant(const FVector2& Position) const
{
	auto	 TilePosition = GetPosition();
	FVector2 TileCenter = { TilePosition.X + TILE_SIZE, TilePosition.Y + TILE_SIZE };

	float X = Position.X > TileCenter.X ? 1.0f : 0.0f;
	float Y = Position.Y > TileCenter.Y ? 1.0f : 0.0f;

	return { X, Y };
}

int PTile::GetQuadrantIndex(const FVector2& Position) const
{
	auto Q = GetQuadrant(Position);
	auto I = ToLinearIndex(Q, 2);
	LogDebug("{} => {}", Q.ToString().c_str(), I);
	return I;
}

json PTile::Serialize() const
{
	json Result;
	Result["Name"] = GetInternalName();
	Result["Class"] = GetClassName();
	Result["Position"] = { Data.X, Data.Y };
	Result["SubIndexes"] = Data.SubIndexes;
	return Result;
}

void PTile::Deserialize(const json& Data)
{
	PActor::Deserialize(Data);
}
