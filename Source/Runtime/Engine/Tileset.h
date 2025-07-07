#pragma once

#include "Core/Vector.h"

#include <map>
#include <string>
#include <vector>

constexpr float gTilesetItemSize = 16.0f;
constexpr int	gTilesetWidth = 16;
constexpr int	gTilesetHeight = 6;

#define TILE_1X1 { gTilesetItemSize, gTilesetItemSize }
#define TILE_2X2 { gTilesetItemSize * 2, gTilesetItemSize * 2 }
#define TILE_3X3 { gTilesetItemSize * 3, gTilesetItemSize * 3 }

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

	STileset() = default;
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

struct STileData
{
	int32_t	  X;
	int32_t	  Y;
	STileset* Tileset;
	int32_t	  Index;

	STilesetItem* GetItem() const { return &Tileset->Items[Index]; }
	ETileType	  GetType() const { return Tileset->Items[Index].Type; }
	PTexture*	  GetTexture() const { return Tileset->Texture; }

	FRect GetSourceRect() const
	{
		auto SourceIndex = Tileset->Items[Index].Index;
		return {
			SourceIndex.X * gTilesetItemSize,
			SourceIndex.Y * gTilesetItemSize,
			gTilesetItemSize,
			gTilesetItemSize,
		};
	}
};

static std::map<std::string, STileset> gTilesets = {
	{
		"Tileset1",
		{
			"Tileset1",
			{
				{ "Grass1", { 0, 0 }, TILE_1X1, TT_Normal },
				{ "Grass2", { 9, 3 }, TILE_1X1, TT_Normal },
				{ "RockTopLeft", { 10, 2 }, TILE_1X1, TT_Obstacle },
				{ "RockTopRight", { 11, 2 }, TILE_1X1, TT_Obstacle },
				{ "RockBottomLeft", { 10, 3 }, TILE_1X1, TT_Obstacle },
				{ "RockBottomRight", { 11, 3 }, TILE_1X1, TT_Obstacle },
			},
		},
	 }
};

static void LoadTileset(const std::string& Name)
{
	auto Tileset = &gTilesets.at(Name);
	auto FileName = Name + ".png";
	auto Tex = PTextureManager::Get(FileName);
	if (!Tex)
	{
		Tex = PTextureManager::Load(FileName);
		if (!Tex)
		{
			LogError("Failed to load tileset {}", Name.c_str());
			return;
		}
	}
	Tileset->Texture = Tex;
}

static STileset& GetTileset(const std::string& Name)
{
	if (!gTilesets.contains(Name))
	{
		LoadTileset(Name);
	}
	auto Tileset = &gTilesets.at(Name);
	if (!Tileset->Texture)
	{
		LoadTileset(Name);
	}
	return *Tileset;
}
