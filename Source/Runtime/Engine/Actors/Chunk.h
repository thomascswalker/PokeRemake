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
	int					mSizeX = 0;
	int					mSizeY = 0;
	STileset*			mTileset = nullptr;

public:
	PChunk() {}
	PChunk(const json& JsonData);
	~PChunk() override;
	void				Start() override;
	void				Draw(const PRenderer* Renderer) const override;
	FRect				GetLocalBounds() const override;
	FRect				GetWorldBounds() const override;
	PTile*				GetTileAtPosition(const FVector2& Position);
	std::vector<PTile*> GetTiles() const { return mTiles; }

	json Serialize() const override;

#if _EDITOR
	bool OnKeyUp(SInputEvent* Event) override;
#endif
};

REGISTER_CLASS(PChunk);