#pragma once

#include "Actor.h"
#include "Engine/Tileset.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

class PChunk;

class PTile : public PActor
{
public:
	STileData Data;
	PChunk*	  Chunk = nullptr;

	PTile() = default;
	PTile(int32_t inX, int32_t inY)
	{
		Data.X = inX;
		Data.Y = inY;
		mRenderPriority = DP_BACKGROUND;
		bSerializable = false;
		mBlocking = false;
		mPosition.X = inX * DOUBLE_TILE_SIZE;
		mPosition.Y = inY * DOUBLE_TILE_SIZE;
	}
	PTile(const STileData& Data)
		: Data(Data)
	{
		mRenderPriority = DP_BACKGROUND;
		bSerializable = false;
		mBlocking = false;
		mPosition.X = Data.X * DOUBLE_TILE_SIZE;
		mPosition.Y = Data.Y * DOUBLE_TILE_SIZE;
	}

	FVector2 GetPosition() const override;
	void	 Draw(const PRenderer* Renderer) const override;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const;
	bool	 Contains(const FVector2& Position) const;

	FRect GetLocalBounds() const override
	{
		return FRect(0, 0, TILE_SIZE, TILE_SIZE);
	}
	FRect GetWorldBounds() const override
	{
		auto P = GetPosition();
		return FRect(P.X, P.Y, TILE_SIZE, TILE_SIZE);
	}

	FVector2 GetQuadrant(const FVector2& Position) const;
	int		 GetQuadrantIndex(const FVector2& Position) const;

	json Serialize() const override;
	void Deserialize(const json& Data) override;
};
