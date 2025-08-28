#pragma once

#include <string>
#include <vector>

#include "Core/Rect.h"

#include "Texture.h"

#include "Core/Array.h"

#define TILESET_1 "Tileset1"
#define TILESET_2 "Tileset2"
#define TILESET_3 "Tileset3"

struct STileItem;
struct SBlock;
struct STileset;

enum ETileType
{
	TT_Normal,
	TT_Obstacle,
	TT_Water,
	TT_Grass,
	TT_Cave,
	TT_Portal
};

struct STileItem
{
	STileset* Tileset = nullptr;
	int32_t Index;

	STileItem() = default;

	STileItem(const int32_t InIndex)
		: Index(InIndex) {}

	FRect GetSourceRect();
};

struct STileset
{
	std::string Name{};
	int32_t Index = 0;
	std::vector<STileItem> Tiles;
	std::vector<int> Blocking;
	float ItemSize    = 8.0f;
	int32_t Width     = 16;
	int32_t Height    = 6;
	PTexture* Texture = nullptr;

	STileset() = default;
	STileset(const std::string& InName, int32_t InWidth, int32_t InHeight,
	         const std::initializer_list<int>& InBlocking);

	int32_t IndexOf(const STileItem* Ptr);

	STileItem* GetTileItem(int32_t Index)
	{
		return &Tiles[Index];
	}

	STileItem* GetTileItem(int32_t X, int32_t Y)
	{
		int32_t Index = Y * Width + X;
		return &Tiles[Index];
	}

	STileItem* GetTileItem(const FVector2& Coordinate)
	{
		return GetTileItem(Coordinate.X, Coordinate.Y);
	}

	auto begin()
	{
		return Tiles.begin();
	}

	auto end()
	{
		return Tiles.end();
	}
};

class TilesetManager
{
public:
	static bool LoadTileset(const std::string& Name);
	static STileset* GetTileset(const std::string& Name);
	static std::vector<STileset*> GetTilesets();
	static STileset* GetDefaultTileset();
	static bool LoadAllTilesets();
	static std::vector<std::string> GetTilesetNames();
};
