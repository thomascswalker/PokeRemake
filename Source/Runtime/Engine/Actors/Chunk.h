#pragma once

#include "Actor.h"
#include "Engine/Tile.h"

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
	PChunk(const SChunkData& Data)
		: mData(Data.Data), mGeometry(Data.Geometry), mTextureName(Data.TextureName)
	{
	}
	~PChunk() override = default;
	void   Start() override;
	void   Draw(const PRenderer* Renderer) const override;
	FRect  GetLocalBounds() const override;
	STile* GetTileAtPosition(const FVector2& Position);
};

DECLARE_STATIC_GLOBAL_GETTER(Chunk)
