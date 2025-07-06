#pragma once

#include "Actor.h"
#include "Engine/ClassRegistry.h"
#include "Engine/Tileset.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

class PChunk;

struct SPortal
{
	std::string DestChunkName;
	IVector2	DestPosition;
};

struct STileData
{
	int32_t	  X;
	int32_t	  Y;
	ETileType Type;
};

class PTile : public PActor
{
public:
	int32_t	  X;
	int32_t	  Y;
	ETileType Type = TT_Normal;
	PTexture* Texture = nullptr;
	PChunk*	  Chunk = nullptr;

	PTile() = default;
	PTile(int32_t inX, int32_t inY)
		: X(inX), Y(inY)
	{
		mPriority = DP_BACKGROUND;
		bSerializable = false;
		bBlocking = false;
		mPosition.X = inX * TILE_SIZE;
		mPosition.Y = inY * TILE_SIZE;
	}
	PTile(const STileData& Data)
		: X(Data.X), Y(Data.Y), Type(Data.Type)
	{
		mPriority = DP_BACKGROUND;
		bSerializable = false;
		bBlocking = false;
		mPosition.X = Data.X * TILE_SIZE;
		mPosition.Y = Data.Y * TILE_SIZE;
	}

	FVector2 GetPosition() const override;
	void	 Draw(const PRenderer* Renderer) const override;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const;
	bool	 Contains(const FVector2& Position) const;

	FRect GetLocalBounds() const override
	{
		return FRect(0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE);
	}
	FRect GetWorldBounds() const override
	{
		auto P = GetPosition();
		return FRect(P.X, P.Y, HALF_TILE_SIZE, HALF_TILE_SIZE);
	}

	json Serialize() const override;
	void Deserialize(const json& Data) override;
};
