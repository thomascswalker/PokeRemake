#pragma once

#include "Actor.h"
#include "Engine/Tile.h"

class PChunk : public PActor
{
	std::vector<STile> mTiles;

public:
	~PChunk() override = default;
	void   Start() override;
	void   Draw(const PRenderer* Renderer) const override;
	FRect  GetLocalBounds() const override;
	STile* GetTileAtPosition(const FVector2& Position);
};

DECLARE_STATIC_GLOBAL_GETTER(Chunk)
