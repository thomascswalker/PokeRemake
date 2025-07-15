#pragma once

#include "Core/Logging.h"
#include "Core/Rect.h"
#include "Core/Vector.h"
#include "Texture.h"

#include <map>
#include <string>
#include <vector>

constexpr float gTilesetItemSize = 8.0f;
constexpr int	gTilesetWidth = 16;
constexpr int	gTilesetHeight = 6;

struct STile;
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

struct STile
{
	STileset*	Tileset = nullptr;
	std::string Name;
	int32_t		Index;
	bool		Blocking = false;

	STile(const std::string& InName, const int32_t InIndex, bool InBlocking)
		: Name(InName), Index(InIndex), Blocking(InBlocking)
	{
	}
	FRect GetSourceRect();
};

struct STileset
{
	std::string		   Name{};
	std::vector<STile> Items;
	std::vector<int>   Blocking;
	int32_t			   Width = 0;
	int32_t			   Height = 0;
	PTexture*		   Texture = nullptr;

	STileset() = default;
	STileset(const std::string& InName, const int32_t InWidth, const int32_t InHeight, const std::initializer_list<STile>& InItems, const std::initializer_list<int>& InBlocking)
		: Name(InName), Width(InWidth), Height(InHeight)
	{
		for (auto Iter = InItems.begin(); Iter != InItems.end(); Iter++)
		{
			Items.push_back(*Iter);
		}
		for (auto Iter = InBlocking.begin(); Iter != InBlocking.end(); Iter++)
		{
			Blocking.push_back(*Iter);
		}
	}

	int32_t IndexOf(const STile* Ptr)
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
			  // Name, Coord, Blocking
			  { "Grass 1", 0, false },
			  { "Grass 2", 57, false },
			  { "Grass 3", 44, false },
			  { "Rock", 42, true },
			  // { "Roof", { 8, 3 }, TT_Obstacle },
			  // { "Roof Top-Left", { 5, 0 }, TT_Obstacle },
			  // { "Roof Bottom-Left 1", { 5, 2 }, TT_Obstacle },
			  // { "Roof Bottom-Left 2", { 6, 2 }, TT_Obstacle },
			  // { "Roof Center 1", { 2, 1 }, TT_Obstacle },
			  // { "Roof Center 2", { 3, 5 }, TT_Obstacle },
			  // { "Roof Center 3", { 7, 0 }, TT_Obstacle },
			  // { "Roof Center 4", { 7, 1 }, TT_Obstacle },
			  // { "Roof Top-Right", { 8, 0 }, TT_Obstacle },
			  // { "Roof Bottom-Right 1", { 8, 2 }, TT_Obstacle },
			  // { "Roof Bottom-Right 2", { 9, 2 }, TT_Obstacle },
			  // { "Roof Left 1", { 5, 1 }, TT_Obstacle },
			  // { "Roof Left 2", { 6, 1 }, TT_Obstacle },
			  // { "Roof Right 1", { 8, 1 }, TT_Obstacle },
			  // { "Roof Right 2", { 9, 1 }, TT_Obstacle },
			  // { "Wall", { 2, 2 }, TT_Obstacle },
			  // { "Wall Left", { 15, 0 }, TT_Obstacle },
			  // { "Wall Right", { 15, 1 }, TT_Obstacle },
			  // { "Wall Left Corner", { 14, 4 }, TT_Obstacle },
			  // { "Wall Right Corner", { 15, 4 }, TT_Obstacle },
			  // { "Wall Bottom", { 10, 1 }, TT_Obstacle },
			  // { "Window", { 10, 0 }, TT_Obstacle },
			  // { "Brick", { 11, 4 }, TT_Obstacle },
			  // { "Sign", { 6, 4 }, TT_Obstacle },
			  // { "Door", { 11, 0 }, TT_Normal },
			  // { "Fence Bottom", { 5, 5 }, TT_Obstacle },
			  // { "Fence Top", { 14, 0 }, TT_Obstacle },
		  },
		  { 42 } },
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
