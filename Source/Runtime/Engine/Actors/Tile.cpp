#include "Tile.h"

#include "../../../Editor/EditorGame.h"
#include "../World.h"
#include "Chunk.h"
#include "Core/Settings.h"

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
	const FRect Source = Data.GetSourceRect();

	// World position of this tile
	const FVector2 WorldPosition = GetPosition();

	// Screenspace destination rectangle
	const FRect Dest = { 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE };

	auto Texture = Data.GetTexture();
	if (!Texture)
	{
		Renderer->SetDrawColor(PColor::UIDebug1);
		Renderer->DrawFillRectAt(Dest, WorldPosition);
	}
	else
	{
		Renderer->DrawTextureAt(Texture, Source, Dest, WorldPosition);
	}

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

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_TileType) && (mMouseOver || mSelected))
	{
		Renderer->SetDrawColor(255, 200, 0, 150);
		if (mMouseOver)
		{
			constexpr float ExpandSize = 2.0f;
			Renderer->DrawRectAt(Dest.Expanded(ExpandSize), WorldPosition - FVector2(ExpandSize, ExpandSize));
		}
		if (mSelected)
		{
			Renderer->DrawFillRectAt(Dest, WorldPosition);
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
	return Position.X >= TilePosition.X						// Min X
		   && Position.X < TilePosition.X + HALF_TILE_SIZE	// Max X
		   && Position.Y >= TilePosition.Y					// Min Y
		   && Position.Y < TilePosition.Y + HALF_TILE_SIZE; // Max Y
}

json PTile::Serialize() const
{
	json Result;
	Result["Name"] = GetInternalName();
	Result["Class"] = GetClassName();
	Result["Position"] = { Data.X, Data.Y };
	Result["Type"] = Data.GetType();
	return Result;
}

void PTile::Deserialize(const json& Data)
{
	PActor::Deserialize(Data);
}
