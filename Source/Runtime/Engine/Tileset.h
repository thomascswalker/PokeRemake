#pragma once

#include "Core/Logging.h"
#include "Core/Rect.h"
#include "Core/Vector.h"
#include "Texture.h"

#include <map>
#include <ranges>
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
	int32_t				   Index = 0;
	std::vector<STileItem> Tiles;
	std::vector<int>	   Blocking;
	float				   ItemSize = 8.0f;
	int32_t				   Width = 16;
	int32_t				   Height = 6;
	PTexture*			   Texture = nullptr;

	STileset() = default;
	STileset(const std::string& InName, const int32_t InWidth, const int32_t InHeight, const std::initializer_list<int>& InBlocking);

	int32_t IndexOf(const STileItem* Ptr);

	auto begin() { return Tiles.begin(); }
	auto end() { return Tiles.end(); }
};

bool					 LoadTileset(const std::string& Name);
STileset*				 GetTileset(const std::string& Name);
std::vector<STileset*>	 GetTilesets();
STileset*				 GetDefaultTileset();
bool					 LoadAllTilesets();
std::vector<std::string> GetTilesetNames();