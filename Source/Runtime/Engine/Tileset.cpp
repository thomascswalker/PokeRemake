#include "Tileset.h"

static std::map<std::string, STileset> gTilesets = {
	{
		TILESET_1,
		{
			TILESET_1, // Name
			16,        // Width
			6,         // Height
			{
				0x01, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0E, 0x0F,                                     //
				0x10, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1E, 0x1F,                         //
				0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2D, 0x2E, 0x2F,       //
				0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x3A, 0x3B, 0x3D, 0x3E, 0x3F,                   //
				0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, //
				0x50, 0x51, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5C, 0x5D,                         //
			},
		},
	},
	{
		TILESET_2,
		{
			TILESET_2, // Name
			16,        // Width
			6,         // Height
			{
				0x00, 0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0E, 0x0F,                                     //
				0x10, 0x12, 0x13, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1E, 0x1F,                                     //
				0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, //
				0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, //
				0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, //
				0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, //
			},
		},
	},
	{
		TILESET_3,
		{
			TILESET_3, // Name
			16,        // Width
			6,         // Height
			{
				0x00, 0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,             //
				0x10, 0x12, 0x13, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,             //
				0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, //
				0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, //
				0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B,                         //
				0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5E, 0x5F,             //
			},
		},
	}
};

FRect STileItem::GetSourceRect()
{
	auto X = Index % Tileset->Width;
	auto Y = Index / Tileset->Width;
	return {
		X * Tileset->ItemSize,
		Y * Tileset->ItemSize,
		Tileset->ItemSize,
		Tileset->ItemSize,
	};
}

STileset::STileset(const std::string& InName, const int32_t InWidth, const int32_t InHeight,
                   const std::initializer_list<int>& InBlocking)
	: Name(InName), Width(InWidth), Height(InHeight)
{
	for (auto Iter = InBlocking.begin(); Iter != InBlocking.end(); Iter++)
	{
		Blocking.push_back(*Iter);
	}

	for (int32_t Index = 0; Index < InWidth * InHeight; Index++)
	{
		Tiles.push_back(STileItem(Index));
	}
}

int32_t STileset::IndexOf(const STileItem* Ptr)
{
	for (int32_t Index = 0; Index < Tiles.size(); Index++)
	{
		auto Item = &Tiles[Index];
		if (Item == Ptr)
		{
			return Index;
		}
	}
	return -1;
}

bool LoadTileset(const std::string& Name)
{
	auto Tileset = &gTilesets.at(Name);
	if (!Tileset)
	{
		LogError("No tileset with name {}", Name.c_str());
		return false;
	}
	auto Tex = PTextureManager::Get(Name);
	if (!Tex)
	{
		// If not found, force a reload
		Tex = PTextureManager::Load(Name + ".png");
		if (!Tex)
		{
			LogError("Failed to load tileset {}", Name.c_str());
			return false;
		}
	}
	for (auto& Tile : Tileset->Tiles)
	{
		Tile.Tileset = Tileset;
	}
	Tileset->Texture = Tex;
	return true;
}

STileset* GetTileset(const std::string& Name)
{
	auto Tileset = &gTilesets.at(Name);
	if (!Tileset)
	{
		// If not found, force a reload
		LoadTileset(Name);
		Tileset = &gTilesets.at(Name);
		if (!Tileset)
		{
			LogError("Failed to get tileset {}", Name.c_str());
			return nullptr;
		}
	}
	return Tileset;
}

std::vector<STileset*> GetTilesets()
{
	std::vector<STileset*> Tilesets;
	for (auto& Tileset : gTilesets | std::views::values)
	{
		Tilesets.push_back(&Tileset);
	}
	return Tilesets;
}

STileset* GetDefaultTileset()
{
	return &gTilesets.at(TILESET_1);
}

bool LoadAllTilesets()
{
	for (const auto& Tileset : gTilesets | std::views::values)
	{
		if (!LoadTileset(Tileset.Name))
		{
			return false;
		}
	}
	return true;
}

std::vector<std::string> GetTilesetNames()
{
	std::vector<std::string> Names;
	for (const auto& Name : gTilesets | std::views::keys)
	{
		Names.push_back(Name);
	}
	return Names;
}
