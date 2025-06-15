#pragma once

#include "Actor.h"
#include "Engine/ClassRegistry.h"
#include "Tile.h"

using TileArray = std::vector<std::vector<int>>;

struct SChunkData
{
	IRect		Geometry;
	int			SizeX;
	int			SizeY;
	std::string TextureName;
	TileArray	Data;
};

class PChunk : public PActor
{
	TileArray			mData{};
	std::vector<PTile*> mTiles;
	IRect				mGeometry;
	std::string			mTextureName;
	int					mSizeX = 0;
	int					mSizeY = 0;

public:
	PChunk() {}
	PChunk(const SChunkData& Data);
	PChunk(const json& JsonData);
	~PChunk() override = default;
	void				Start() override;
	void				Draw(const PRenderer* Renderer) const override;
	FRect				GetLocalBounds() const override;
	FRect				GetWorldBounds() const override;
	PTile*				GetTileAtPosition(const FVector2& Position);
	std::vector<PTile*> GetTiles() const { return mTiles; }

	json Serialize() const override;
};

REGISTER_CLASS(PChunk);