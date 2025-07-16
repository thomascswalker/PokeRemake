#pragma once

#include "Core/Logging.h"
#include "Core/Rect.h"
#include "Core/Vector.h"
#include "Texture.h"

#include <map>
#include <string>
#include <vector>

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
	int32_t	  Index;

	STileItem(const int32_t InIndex)
		: Index(InIndex)
	{
	}
	FRect GetSourceRect();
};

struct STileset
{
	std::string			   Name{};
	std::vector<STileItem> Items;
	std::vector<int>	   Blocking;
	float				   ItemSize = 8.0f;
	int32_t				   Width = 16;
	int32_t				   Height = 6;
	PTexture*			   Texture = nullptr;

	STileset() = default;
	STileset(const std::string& InName, const int32_t InWidth, const int32_t InHeight, const std::initializer_list<int>& InBlocking)
		: Name(InName), Width(InWidth), Height(InHeight)
	{
		for (auto Iter = InBlocking.begin(); Iter != InBlocking.end(); Iter++)
		{
			Blocking.push_back(*Iter);
		}

		for (int32_t Index = 0; Index < InWidth * InHeight; Index++)
		{
			Items.push_back(STileItem(Index));
		}
	}

	int32_t IndexOf(const STileItem* Ptr)
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

static std::map<std::string, STileset> gTilesets = {
	{
		"Tileset1",
		{ "Tileset1", // Name
		  16,		  // Width
		  6,		  // Height
		  {
			  42, 43, 58, 59, // Rock
		  } },
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
	for (auto& Item : Tileset->Items)
	{
		Item.Tileset = Tileset;
	}
	Tileset->Texture = Tex;
}

static STileset* GetTileset(const std::string& Name)
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
	return Tileset;
}
