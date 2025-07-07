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
	TST_Full,
	TST_Half
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
	ETileSizeType SizeType = TST_Full;

	STilesetItem(const std::string& InName, const FVector2& InIndex, ETileType InType, ETileSizeType InSizeType)
		: Name(InName), LinearIndex(ToLinearIndex(InIndex)), Type(InType), SizeType(InSizeType)
	{
	}

	FRect GetSourceRect()
	{
		auto CoordIndex = ToCoordIndex(LinearIndex);
		auto Size = SizeType == TST_Half ? gTilesetItemHalfSize : gTilesetItemSize;
		return {
			CoordIndex * gTilesetItemHalfSize, { Size, Size }
		};
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
	ETileType GetType(int Index = 0) const
	{
		return GetItemBySubIndex(Index)->Type;
	}
	PTexture* GetTexture() const
	{
		return Tileset->Texture;
	}
	FRect GetSourceRect(int Index = 0) const
	{
		return GetItemBySubIndex(Index)->GetSourceRect();
	}
	bool IsSubIndexed() const { return GetItemBySubIndex(0)->SizeType == TST_Half; }
};

static std::map<std::string, STileset> gTilesets = {
	{
		"Tileset1",
		{
			"Tileset1",
			{
				{ "Grass1", { 0, 0 }, TT_Normal, TST_Half },
				{ "Grass2", { 9, 3 }, TT_Normal, TST_Half },
				{ "Rock", { 10, 2 }, TT_Obstacle, TST_Full },
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
