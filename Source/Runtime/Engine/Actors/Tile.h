#pragma once

#include "Actor.h"
#include "Core/Delegate.h"
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

class PTile;

#if _EDITOR
DECLARE_MULTICAST_DELEGATE(DTileClicked, PTile*);
#endif

class PTile : public PActor
{
public:
	int32_t	  X;
	int32_t	  Y;
	ETileType Type = TT_Normal;
	PTexture* Texture = nullptr;
	PChunk*	  Chunk = nullptr;
#if _EDITOR
	DTileClicked TileClicked;
#endif
	PTile(int32_t inX, int32_t inY) : X(inX), Y(inY) {}

	FVector2 GetLocalPosition() const;
	FVector2 GetWorldPosition() const;
	void	 Draw(const PRenderer* Renderer) const override;
	void	 Tick(float DeltaTime) override;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const;

	bool Contains(const FVector2& Position) const;

	// Editor-only functions
#if _EDITOR
	bool IsMouseOver() const override;
#endif
};
