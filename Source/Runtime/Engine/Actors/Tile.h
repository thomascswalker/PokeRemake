#pragma once

#include "Actor.h"
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

struct STile
{
	int32_t	  X;
	int32_t	  Y;
	ETileType Type = TT_Normal;
	PTexture* Texture = nullptr;
	PChunk*	  Chunk = nullptr;

	STile(int32_t inX, int32_t inY) : X(inX), Y(inY) {}

	FVector2 GetLocalPosition() const;
	FVector2 GetWorldPosition() const;
	void	 Draw(const PRenderer* Renderer) const;
	PActor*	 GetActor() const;
	bool	 IsWalkable() const;

	bool Contains(const FVector2& Position) const;
};
