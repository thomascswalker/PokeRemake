#pragma once

#include "Core/Vector.h"

#include <map>
#include <string>
#include <vector>

constexpr float gTilesetItemSize = 32.0f;
constexpr float gTilesetItemHalfSize = gTilesetItemSize / 2.0f;
constexpr int	gTilesetWidth = 16;
constexpr int	gTilesetHeight = 6;

#define TILE_HALF { gTilesetItemHalfSize, gTilesetItemHalfSize }
#define TILE_FULL { gTilesetItemSize, gTilesetItemSize }

struct STileset;
struct STilesetItem;

enum ETileSizeType
{
	TST_1X1,
	TST_2X2,
};

enum ETileType
{
	TT_Normal,
	TT_Obstacle,
	TT_Water,
	TT_Grass,
	TT_Cave,
	TT_Portal
};

static int32_t ToLinearIndex(const FVector2& Index, const int32_t Factor = gTilesetWidth)
{
	return Index.Y * Factor + Index.X;
}

static FVector2 ToCoordIndex(const int32_t Index, const int32_t Factor = gTilesetWidth)
{
	return { static_cast<float>(Index % Factor), static_cast<float>(Index / Factor) };
}

struct STilesetItem
{
	std::string	  Name;
	int32_t		  LinearIndex;
	ETileType	  Type = TT_Normal;
	ETileSizeType SizeType = TST_2X2;

	STilesetItem(const std::string& InName, const FVector2& InIndex, ETileType InType, ETileSizeType InSizeType)
		: Name(InName), LinearIndex(ToLinearIndex(InIndex)), Type(InType), SizeType(InSizeType)
	{
	}

	FRect GetSourceRect()
	{
		auto CoordIndex = ToCoordIndex(LinearIndex);
		auto Factor = SizeType == TST_1X1 ? gTilesetItemHalfSize : gTilesetItemSize;
		return {
			CoordIndex * gTilesetItemHalfSize, { Factor, Factor }
		};
	}

	FRect GetDestRect()
	{
		auto	 CoordIndex = ToCoordIndex(LinearIndex);
		FVector2 Size;
		switch (SizeType)
		{
			case TST_1X1:
				Size = { HALF_TILE_SIZE,
						 HALF_TILE_SIZE };
				break;
			case TST_2X2:
				Size = { TILE_SIZE,
						 TILE_SIZE };
				break;
		}

		return { CoordIndex * HALF_TILE_SIZE, Size };
	}
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

	STilesetItem* GetItemByLinearIndex(int LinearIndex)
	{
		for (auto& Item : Items)
		{
			if (Item.LinearIndex == LinearIndex)
			{
				return &Item;
			}
		}
		return nullptr;
	}

	int32_t IndexOf(const STilesetItem* Ptr)
	{
		for (int32_t Index = 0; Index < Items.size(); Index++)
		{
			auto Item = &Items[Index];
			if (Item == Ptr)
			{
				return Index;
			}
		}
		return -1;
	}

	auto begin() { return Items.begin(); }
	auto end() { return Items.end(); }
};

struct STileData
{
	using SubIndexType = std::array<int32_t, 4>;

	int32_t		 X;
	int32_t		 Y;
	STileset*	 Tileset;
	SubIndexType SubIndexes = { 0, 0, 0, 0 };

	STilesetItem* GetItemBySubIndex(int Index = 0) const
	{
		return Tileset->GetItemByLinearIndex(SubIndexes[Index]);
	}
	ETileType GetType() const
	{
		return GetItemBySubIndex(0)->Type;
	}
	PTexture* GetTexture() const
	{
		return Tileset->Texture;
	}
	FRect GetSourceRect(int Index = 0) const
	{
		auto Item = GetItemBySubIndex(Index);
		if (!Item)
		{
			return {};
		}
		return Item->GetSourceRect();
	}
};

static std::map<std::string, STileset> gTilesets = {
	{
		"Tileset1",
		{
			"Tileset1",
			{
				{ "Grass 1", { 0, 0 }, TT_Normal, TST_1X1 },
				{ "Grass 2", { 9, 3 }, TT_Normal, TST_1X1 },
				{ "Grass 3", { 12, 2 }, TT_Normal, TST_1X1 },
				{ "Rock", { 10, 2 }, TT_Obstacle, TST_2X2 },
				{ "Roof", { 8, 3 }, TT_Obstacle, TST_1X1 },
				{ "Roof Top-Left", { 5, 0 }, TT_Obstacle, TST_2X2 },
				{ "Roof Bottom-Left 1", { 5, 2 }, TT_Obstacle, TST_1X1 },
				{ "Roof Bottom-Left 2", { 6, 2 }, TT_Obstacle, TST_1X1 },
				{ "Roof Center 1", { 2, 1 }, TT_Obstacle, TST_1X1 },
				{ "Roof Center 2", { 3, 5 }, TT_Obstacle, TST_1X1 },
				{ "Roof Center 3", { 7, 0 }, TT_Obstacle, TST_1X1 },
				{ "Roof Center 4", { 7, 1 }, TT_Obstacle, TST_1X1 },
				{ "Roof Top-Right", { 8, 0 }, TT_Obstacle, TST_2X2 },
				{ "Roof Bottom-Right 1", { 8, 2 }, TT_Obstacle, TST_1X1 },
				{ "Roof Bottom-Right 2", { 9, 2 }, TT_Obstacle, TST_1X1 },
				{ "Roof Left 1", { 5, 1 }, TT_Obstacle, TST_1X1 },
				{ "Roof Left 2", { 6, 1 }, TT_Obstacle, TST_1X1 },
				{ "Roof Right 1", { 8, 1 }, TT_Obstacle, TST_1X1 },
				{ "Roof Right 2", { 9, 1 }, TT_Obstacle, TST_1X1 },
				{ "Wall", { 2, 2 }, TT_Obstacle, TST_1X1 },
				{ "Wall Left", { 15, 0 }, TT_Obstacle, TST_1X1 },
				{ "Wall Right", { 15, 1 }, TT_Obstacle, TST_1X1 },
				{ "Wall Left Corner", { 14, 4 }, TT_Obstacle, TST_1X1 },
				{ "Wall Right Corner", { 15, 4 }, TT_Obstacle, TST_1X1 },
				{ "Wall Bottom", { 10, 1 }, TT_Obstacle, TST_1X1 },
				{ "Window", { 10, 0 }, TT_Obstacle, TST_1X1 },
				{ "Brick", { 11, 4 }, TT_Obstacle, TST_1X1 },
				{ "Sign", { 6, 4 }, TT_Obstacle, TST_2X2 },
				{ "Door", { 11, 0 }, TT_Normal, TST_2X2 },
				{ "Fence Bottom", { 5, 5 }, TT_Obstacle, TST_1X1 },
				{ "Fence Top", { 14, 0 }, TT_Obstacle, TST_1X1 },
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
	// ReSharper disable once CppDFAUnusedValue
	auto Tileset = &gTilesets.at(Name);
	if (!Tileset->Texture)
	{
		LoadTileset(Name);
	}
	return *Tileset;
}
