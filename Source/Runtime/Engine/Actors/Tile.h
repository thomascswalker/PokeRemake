#pragma once

#include "Actor.h"
#include "Core/Delegate.h"
#include "Engine/ClassRegistry.h"
#include "Renderer/Renderer.h"

constexpr int CHUNK_SIZE = 20;

class PChunk;

enum ETileType
{
	TT_Normal,
	TT_Obstacle,
	TT_Water,
	TT_Grass,
	TT_Cave,
	TT_Portal
};

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
#if _EDITOR
	bool bSelected = false;
#endif

	int32_t	  X;
	int32_t	  Y;
	ETileType Type = TT_Normal;
	PTexture* Texture = nullptr;
	PChunk*	  Chunk = nullptr;

	PTile() = default;
	PTile(const json& JsonData);
	PTile(int32_t inX, int32_t inY) : X(inX), Y(inY)
	{
		bSerializable = false;
		bBlocking = false;
		mPosition.X = inX * TILE_SIZE;
		mPosition.Y = inY * TILE_SIZE;
	}
	PTile(const STileData& Data) : X(Data.X), Y(Data.Y), Type(Data.Type)
	{
		bSerializable = false;
		bBlocking = false;
		mPosition.X = Data.X * TILE_SIZE;
		mPosition.Y = Data.Y * TILE_SIZE;
	}

	FVector2 GetPosition() const override;
	FVector2 GetLocalPosition() const;
	FVector2 GetWorldPosition() const;
	void	 Draw(const PRenderer* Renderer) const override;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const;
	bool	 Contains(const FVector2& Position) const;

	json Serialize() const override;
	void Deserialize(const json& Data) override;

#if _EDITOR
	void SetSelected(bool State) { bSelected = State; }
	bool GetSelected() { return bSelected; }
#endif
};

REGISTER_CLASS(PTile);