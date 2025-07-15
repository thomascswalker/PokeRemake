#pragma once

#include "Actor.h"
#include "Engine/Tileset.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

class PChunk;

class PTile : public PActor
{
public:
	int32_t X = 0;
	int32_t Y = 0;

	STileset* Tileset = nullptr;
	int32_t	  TilesetIndex = 0;

	PChunk* Chunk = nullptr;

	PTile() = default;
	PTile(int32_t inX, int32_t inY)
	{
		X = inX;
		Y = inY;
		mRenderPriority = DP_BACKGROUND;
		bSerializable = false;
		mBlocking = false;
		mPosition.X = inX * BLOCK_SIZE;
		mPosition.Y = inY * BLOCK_SIZE;
	}

	void	Draw(const PRenderer* Renderer) const override;
	void	DebugDraw(const PRenderer* Renderer) const override;
	PActor* GetActor() const;
	bool	IsWalkable() const;
	bool	Contains(const FVector2& Position) const;

	FRect GetLocalBounds() const override
	{
		return FRect(0, 0, TILE_SIZE, TILE_SIZE);
	}
	FRect GetWorldBounds() const override
	{
		auto P = GetWorldPosition();
		return FRect(P.X, P.Y, TILE_SIZE, TILE_SIZE);
	}

	FVector2 GetPosition() const override;

	PTile* GetAdjacent(int32_t InX, int32_t InY);

	// Only checks the bottom-left tile
	bool IsBlocking() const override
	{
		return std::ranges::find(Tileset->Blocking, TilesetIndex) != Tileset->Blocking.end();
	}

	json Serialize() const override;
	void Deserialize(const json& Data) override;
};
