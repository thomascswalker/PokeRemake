// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "../../../Editor/EditorGame.h"
#include "Engine/ClassRegistry.h"
#include "Engine/World.h"

PChunk::PChunk(const SChunkData& Data)
	: mData(Data.Data), mSizeX(Data.SizeX), mSizeY(Data.SizeY), mTextureName(Data.TextureName)
{
	mPriority = DP_BACKGROUND;
	bBlocking = false;
	mPosition = FVector2(mSizeX * TILE_SIZE, mSizeY * TILE_SIZE);
}

PChunk::PChunk(const json& JsonData)
{
	LogDebug("Constructing Chunk from JSON data");
	mPriority = DP_BACKGROUND;
	bBlocking = false;

	auto Position = JsonData["Position"];
	mPosition = FVector2(Position[0].get<float>(), Position[1].get<float>());

	mSizeX = JsonData["SizeX"].get<int>();
	mSizeY = JsonData["SizeY"].get<int>();
	mData = std::vector(mSizeX, std::vector(mSizeY, 0));

	auto Tiles = JsonData["Tiles"];
	for (const auto& T : Tiles)
	{
		auto X = T["Position"][0].get<int>();
		auto Y = T["Position"][1].get<int>();
		auto Type = T["Type"].get<int>();
		mData[X][Y] = Type;
	}
}

void PChunk::Start()
{
	PTexture* T = nullptr;
	if (!mTextureName.empty())
	{
		T = PTextureManager::Load(mTextureName);
		mSprite.SetTexture(T);
	}

	auto W = GetWorld();

	// Instantiate each tile in the grid
	for (int X = 0; X < mSizeX; X++)
	{
		for (int Y = 0; Y < mSizeY; Y++)
		{
			const auto Tile = W->ConstructActor<PTile>(X, Y);
			Tile->Chunk = this;
			Tile->Type = static_cast<ETileType>(mData[Y][X]); // Access [Row][Column]
			Tile->Texture = T;
			mTiles.emplace_back(Tile);
		}
	}

#if _EDITOR
	if (PEditorGame* EditorGame = dynamic_cast<PEditorGame*>(GetGame()))
	{
		EditorGame->AddChunk(this);
	}
#endif
}

void PChunk::Draw(const PRenderer* Renderer) const
{
	for (const auto& Tile : mTiles)
	{
		Tile->Draw(Renderer);
	}
}

FRect PChunk::GetLocalBounds() const
{
	return { 0, 0, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE };
}

FRect PChunk::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE };
}

PTile* PChunk::GetTileAtPosition(const FVector2& Position)
{
	for (auto& Tile : mTiles)
	{
		if (Tile->Contains(Position))
		{
			// Return a pointer to the tile
			return Tile;
		}
	}
	return nullptr; // No tile found at the given position
}

json PChunk::Serialize() const
{
	json Result;
	Result["Name"] = GetInternalName();
	Result["Class"] = GetClassName();
	Result["Position"] = { GetPosition().X, GetPosition().Y };

	Result["SizeX"] = mSizeX;
	Result["SizeY"] = mSizeY;

	auto TileArray = json::array();
	for (const auto& Tile : GetTiles())
	{
		TileArray.push_back(Tile->Serialize());
	}
	Result["Tiles"] = TileArray;
	return Result;
}
