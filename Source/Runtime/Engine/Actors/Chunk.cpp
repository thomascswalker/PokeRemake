// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "Engine/World.h"

PChunk::PChunk(const SChunkData& Data)
	: mData(Data.Data), mGeometry(Data.Geometry), mTextureName(Data.TextureName)
{
	mPriority = DP_BACKGROUND;
	bBlocking = false;
	mPosition = FVector2(mGeometry.X * TILE_SIZE, mGeometry.Y * TILE_SIZE);
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
	for (int X = 0; X < mGeometry.W; X++)
	{
		for (int Y = 0; Y < mGeometry.H; Y++)
		{
			const auto Tile = W->ConstructActor<PTile>(X, Y);
			Tile->Chunk = this;
			Tile->Type = static_cast<ETileType>(mData[Y][X]); // Access [Row][Column]
			Tile->Texture = T;
			mTiles.emplace_back(Tile);
		}
	}
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
	return { 0, 0, mGeometry.W * TILE_SIZE, mGeometry.H * TILE_SIZE };
}

FRect PChunk::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, mGeometry.W * TILE_SIZE, mGeometry.H * TILE_SIZE };
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
