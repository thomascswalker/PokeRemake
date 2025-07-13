#pragma once

#include "Core/Files.h"
#include "Core/Json.h"
#include "Core/Rect.h"
#include "Core/Vector.h"
#include "Engine/Texture.h"

#include <map>
#include <string>
#include <vector>

constexpr float gTilesetItemSize = 16.0f;
constexpr float gTilesetItemHalfSize = gTilesetItemSize / 2.0f;

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

template <typename T>
static int32_t ToLinearIndex(const TVector2<T>& Index, const int32_t Factor = 16)
{
	return Index.Y * Factor + Index.X;
}

template <typename T>
static TVector2<T> ToCoordIndex(const int32_t Index, const int32_t Factor = 16)
{
	return { static_cast<float>(Index % Factor), static_cast<float>(Index / Factor) };
}

struct STilesetItem
{
	std::string	  Name;
	int32_t		  LinearIndex;
	ETileType	  Type = TT_Normal;
	ETileSizeType SizeType = TST_2X2;

	STilesetItem() = default;
	STilesetItem(const std::string& InName, const FVector2& InIndex, ETileType InType, ETileSizeType InSizeType)
		: Name(InName), LinearIndex(ToLinearIndex(InIndex)), Type(InType), SizeType(InSizeType)
	{
	}

	FRect GetSourceRect()
	{
		auto CoordIndex = ToCoordIndex<float>(LinearIndex);
		auto Factor = SizeType == TST_1X1 ? gTilesetItemHalfSize : gTilesetItemSize;
		return {
			CoordIndex * gTilesetItemHalfSize, { Factor, Factor }
		};
	}

	FRect GetDestRect()
	{
		auto	 CoordIndex = ToCoordIndex<float>(LinearIndex);
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
	int32_t					  Scale = 8;
	int32_t					  Width = 0;
	int32_t					  Height = 0;

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
		if (!Tileset || !Tileset->Texture)
		{
			return nullptr;
		}
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
	void SetSubIndexes(const SubIndexType& InSubIndexes)
	{
		for (int32_t Index = 0; Index < 4; Index++)
		{
			if (InSubIndexes[Index] < 0)
			{
				continue;
			}
			SubIndexes[Index] = InSubIndexes[Index];
		}
	}
};

class PTilesetManager
{
	std::map<std::string, STileset> mTilesets{};
	static PTilesetManager*			mInstance;

public:
	static PTilesetManager* GetInstance()
	{
		if (!mInstance)
		{
			mInstance = new PTilesetManager();
		}
		return mInstance;
	}

	void Load(const std::string& Name)
	{
		LogDebug("Loading tileset {}", Name.c_str());
		auto TilesetJSONFile = Files::FindFile(Name + ".JSON");

		if (TilesetJSONFile.empty())
		{
			LogWarning("Tileset JSON {} not found", Name.c_str());
			return;
		}
		LogDebug("Found tileset JSON file: {}", TilesetJSONFile.c_str());
		std::string JsonData;
		Files::ReadFile(TilesetJSONFile, JsonData);
		auto TilesetData = JSON::parse(JsonData);

		auto TextureName = Name + ".png";
		auto Tex = PTextureManager::Get(TextureName);
		if (!Tex)
		{
			Tex = PTextureManager::Load(TextureName);
			if (!Tex)
			{
				LogError("Failed to load tileset texture {}", TextureName.c_str());
				return;
			}
		}

		STileset NewTileset;
		NewTileset.Name = TextureName;
		NewTileset.Texture = Tex;
		NewTileset.Scale = TilesetData.at("Scale").get<int32_t>();
		NewTileset.Width = TilesetData.at("Width").get<int32_t>();
		NewTileset.Height = TilesetData.at("Height").get<int32_t>();

		for (const auto& Tiles : TilesetData.at("Tiles"))
		{
			STilesetItem Item;
			Item.Name = Tiles.at("Name").get<std::string>();
			FVector2 ItemIndex;
			ItemIndex.X = Tiles.at("Coord").at(0).get<int32_t>();
			ItemIndex.Y = Tiles.at("Coord").at(1).get<int32_t>();
			Item.LinearIndex = ToLinearIndex(ItemIndex, NewTileset.Width);
			Item.Type = Tiles.at("Type").get<ETileType>();
			Item.SizeType = Tiles.at("Size").get<ETileSizeType>();
			NewTileset.Items.emplace_back(Item);
		}

		mTilesets[Name] = NewTileset;
	}

	STileset* Get(const std::string& Name)
	{
		if (!mTilesets.contains(Name))
		{
			LogError("Tileset {} not found. Perhaps it's not loaded?", Name.c_str());
			LogError("Available tilesets:");
			for (const auto& val : mTilesets | std::views::values)
			{
				LogError("\t{}", val.Name.c_str());
			}
			return nullptr;
		}
		auto Tileset = &mTilesets.at(Name);
		if (!Tileset->Texture)
		{
			Load(Name);
		}
		return Tileset;
	}
};