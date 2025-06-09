#pragma once

#include "Actor.h"
#include "Tile.h"

using TileArray = std::vector<std::vector<int>>;

struct SChunkData
{
	IRect		Geometry;
	std::string TextureName;
	TileArray	Data;
};

class PChunk : public PActor
{
	TileArray mData;

	std::vector<STile> mTiles;
	IRect			   mGeometry;
	std::string		   mTextureName;

public:
	explicit PChunk(const SChunkData& Data);
	~PChunk() override = default;
	void   Start() override;
	void   Draw(const PRenderer* Renderer) const override;
	FRect  GetLocalBounds() const override;
	FRect  GetWorldBounds() const override;
	STile* GetTileAtPosition(const FVector2& Position);
};
