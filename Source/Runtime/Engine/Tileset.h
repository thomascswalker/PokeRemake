#pragma once
#include "Core/Vector.h"

#include <string>

#define TILE_1X1 { 8, 8 }
#define TILE_2X2 { 16, 16 }
#define TILE_3X3 { 24, 24 }

struct STileset;
struct STilesetItem;

enum ETileType
{
	TT_Normal,
	TT_Obstacle,
	TT_Water,
	TT_Grass,
	TT_Cave,
	TT_Portal
};

struct STilesetItem
{
	std::string Name;
	FVector2	Index = { 0, 0 };
	FVector2	Size = TILE_1X1;
	ETileType	Type = TT_Normal;
};

struct STileset
{
	std::vector<STilesetItem> Items;
};