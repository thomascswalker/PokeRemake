#pragma once

#include "Actor.h"
#include "Engine/Tileset.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

class PChunk;

class PTile : public PActor
{
public:
	STileData Data{};
	PChunk*	  Chunk = nullptr;
	int32_t	  X;
	int32_t	  Y;

	PTile() = default;
	PTile(int32_t InX, int32_t InY)
	{
		X = InX;
		Y = InY;
		mRenderPriority = DP_BACKGROUND;
		bSerializable = false;
		mBlocking = false;
		mPosition.X = InX * DOUBLE_TILE_SIZE;
		mPosition.Y = InY * DOUBLE_TILE_SIZE;
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

	PTile*	 GetAdjacent(int32_t InX, int32_t InY);
	FVector2 GetQuadrant(const FVector2& Position) const;
	int		 GetQuadrantIndex(const FVector2& Position) const;

	json Serialize() const override;
	void Deserialize(const json& Data) override;
};
