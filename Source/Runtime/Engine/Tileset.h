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
			  0x01, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0E, 0x0F,									  //
			  0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1E, 0x1F,						  //
			  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2D, 0x2E, 0x2F,		  //
			  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x3A, 0x3B, 0x3D, 0x3E, 0x3F,					  //
			  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, //
			  0x50, 0x51, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5C, 0x5D,						  //
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
