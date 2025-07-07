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
	std::string				  Name{};
	std::vector<STilesetItem> Items;
	PTexture*				  Texture = nullptr;

	STileset(const std::string& InName)
		: Name(InName) {}
	STileset(const std::string& InName, const std::initializer_list<STilesetItem>& InItems)
		: Name(InName)
	{
		for (auto Iter = InItems.begin(); Iter != InItems.end(); Iter++)
		{
			Items.push_back(*Iter);
		}
	}

	auto begin() { return Items.begin(); }
	auto end() { return Items.end(); }
};

static STileset gPalletTownTileset = {
	"Pallet Town",
	{
		{ "Grass1", { 0, 0 }, TILE_1X1, TT_Normal },
		{ "Grass2", { 9, 3 }, TILE_1X1, TT_Normal },
		{ "Rock1", { 10, 2 }, TILE_2X2, TT_Obstacle },
	  },
};
