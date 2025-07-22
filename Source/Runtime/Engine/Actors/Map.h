#pragma once

#include "Actor.h"
#include "Core/Json.h"

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

	bool	 IsBlocking() const;
	FVector2 GetPosition() const;
	FRect	 GetSourceRect() const;
	FRect	 GetDestRect() const;
};

class PMap : public PActor
{
	std::string		   mMapName;
	std::vector<STile> mTiles;

	// Width of this map in tiles
	int mSizeX = 0;
	// Height of this map in tiles
	int mSizeY = 0;

public:
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

	std::string GetMapName() const { return mMapName; }
	void		SetMapName(const std::string& MapName) { mMapName = MapName; }

	json Serialize() const override;
	void Deserialize(const json& JsonData) override;

#if _EDITOR
	void OnKeyUp(SInputEvent* Event) override;
#endif
};
