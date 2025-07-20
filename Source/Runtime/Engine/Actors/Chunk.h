#pragma once

#include "Actor.h"
#include "Core/Json.h"
#include "Engine/ClassRegistry.h"

class PChunk;

struct STile
{
	// The tileset this tile is associated with.
	STileset* Tileset;
	// The chunk this tile belongs to.
	PChunk* Chunk = nullptr;
	// The index within the tileset this tile will render.
	int32_t Index;
	// The X coordinate within the chunk.
	int32_t X;
	// The Y coordinate within the chunk.
	int32_t Y;

	bool	 IsBlocking() const;
	FVector2 GetPosition() const;
	FRect	 GetSourceRect() const;
	FRect	 GetDestRect() const;
};

class PChunk : public PActor
{
	JSON			   mData;
	std::vector<STile> mTiles;

	// Width of this chunk in tiles
	int mSizeX = 0;
	// Height of this chunk in tiles
	int mSizeY = 0;

public:
	PChunk() {}
	PChunk(const json& JsonData);

	void  Start() override;
	bool  Draw(const PRenderer* Renderer) const override;
	bool  DebugDraw(const PRenderer* Renderer) const override;
	FRect GetLocalBounds() const override;
	FRect GetWorldBounds() const override;

	std::vector<STile*> GetTiles();
	STile*				GetTile(int Index);
	STile*				GetTileAt(int X, int Y) const;
	STile*				GetTileAtPosition(const FVector2& Position) const;
	STile*				GetTileUnderMouse() const;

	int		 GetSizeX() const { return mSizeX; }
	int		 GetSizeY() const { return mSizeY; }
	IVector2 GetSize() const { return { mSizeX, mSizeY }; }

	json Serialize() const override;

#if _EDITOR
	void OnKeyUp(SInputEvent* Event) override;
#endif
};

REGISTER_CLASS(PChunk);