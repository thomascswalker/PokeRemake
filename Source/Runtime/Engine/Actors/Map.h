#pragma once

#include "Core/CoreFwd.h"

#include "Actor.h"

class PMap;

struct STile
{
	// The tileset this tile is associated with.
	STileset* Tileset;
	// The map this tile belongs to.
	PMap* Map = nullptr;
	// The index within the tileset this tile will render.
	int32_t Index;
	// The X coordinate within the map.
	int32_t X;
	// The Y coordinate within the map.
	int32_t Y;

	bool IsBlocking() const;
	FVector2 GetPosition() const;
	FRect GetSourceRect() const;
	FRect GetDestRect() const;
};

class PMap : public PActor, public IDrawable
{
	// The display name of the map
	std::string mMapName;
	// The array of tiles within this map
	std::vector<STile> mTiles;
	// Width of this map in tiles
	int32_t mSizeX = 0;
	// Height of this map in tiles
	int32_t mSizeY = 0;

public:
	PMap();
	void Start() override;
	bool Draw(const PRenderer* Renderer) const override;
	bool DebugDraw(const PRenderer* Renderer) const override;
	FRect GetLocalBounds() const override;
	FRect GetWorldBounds() const override;

	float GetDepth() const override
	{
		return mPosition.Z;
	}

	std::vector<STile*> GetTiles();
	STile* GetTile(int Index);
	STile* GetTileAt(int X, int Y) const;
	STile* GetTileAtPosition(const FVector2& Position) const;
	STile* GetTileUnderMouse() const;

	int GetSizeX() const
	{
		return mSizeX;
	}

	int GetSizeY() const
	{
		return mSizeY;
	}

	IVector2 GetSize() const
	{
		return {mSizeX, mSizeY};
	}

	std::string GetMapName() const
	{
		return mMapName;
	}

	void SetMapName(const std::string& MapName)
	{
		mMapName = MapName;
	}

	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;
};
