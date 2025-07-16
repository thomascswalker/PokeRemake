#pragma once

#include "Actor.h"
#include "Core/Json.h"
#include "Engine/ClassRegistry.h"
#include "Tile.h"

using TileArray = std::vector<std::vector<int>>;

class PChunk : public PActor
{
	JSON				mData;
	std::vector<PTile*> mTiles;
	IRect				mGeometry;
	std::string			mTextureName;
	// Width of this chunk in tiles
	int mSizeX = 0;
	// Height of this chunk in tiles
	int		  mSizeY = 0;
	STileset* mTileset = nullptr;

public:
	PChunk() {}
	PChunk(const json& JsonData);
	~PChunk() override;
	void				Start() override;
	void				DebugDraw(const PRenderer* Renderer) const override;
	FRect				GetLocalBounds() const override;
	FRect				GetWorldBounds() const override;
	PTile*				GetTileAtPosition(const FVector2& Position) const;
	SBlock				GetBlockAtPosition(const FVector2& Position) const;
	PTile*				GetTileAt(int X, int Y) const;
	std::vector<PTile*> GetTiles() const { return mTiles; }
	PTile*				GetTile(int Index);
	int					GetSizeX() const { return mSizeX; }
	int					GetSizeY() const { return mSizeY; }
	IVector2			GetSize() const { return { mSizeX, mSizeY }; }
	std::vector<PTile*> GetAdjacentBlocks(const PTile* OriginBlock, int32_t BrushSize) const;

	json Serialize() const override;

#if _EDITOR
	void OnKeyUp(SInputEvent* Event) override;
#endif
};

REGISTER_CLASS(PChunk);