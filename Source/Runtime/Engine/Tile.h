#pragma once

#include "Actors/Actor.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

enum ETileType
{
	TT_Normal,
	TT_Obstacle,
	TT_Water,
	TT_Grass,
	TT_Cave,
	TT_Portal
};

struct STile
{
	int32_t	  X;
	int32_t	  Y;
	ETileType Type = TT_Normal;
	PTexture* Texture = nullptr;

	STile(int32_t inX, int32_t inY) : X(inX), Y(inY) {}

	FVector2 GetPosition() const { return FVector2(X * TILE_SIZE, Y * TILE_SIZE); }
	void	 Draw(const PRenderer* Renderer, const FVector2& Offset) const;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const { return Type != TT_Obstacle && Type != TT_Water; }

	bool Contains(const FVector2& Position) const
	{
		auto TilePosition = GetPosition();
		return Position.X >= TilePosition.X						// Min X
			   && Position.X < TilePosition.X + HALF_TILE_SIZE	// Max X
			   && Position.Y >= TilePosition.Y					// Min Y
			   && Position.Y < TilePosition.Y + HALF_TILE_SIZE; // Max Y
	}
};
