// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "Core/Settings.h"
#include "Engine/ClassRegistry.h"
#include "Engine/World.h"
#if _EDITOR
#include "../../../Editor/EditorGame.h"
#endif

PChunk::PChunk(const json& JsonData)
{
	LogDebug("Constructing Chunk from JSON data");
	mRenderPriority = DP_BACKGROUND;
	mBlocking = false;
	mData = JsonData;
}

PChunk::~PChunk()
{
	auto W = GetWorld();
	for (auto Tile : mTiles)
	{
		W->DestroyActor(Tile);
	}
}

void PChunk::Start()
{
	auto W = GetWorld();

	auto Position = mData.at("Position");
	mPosition = FVector2(Position[0].get<float>(), Position[1].get<float>());

	mSizeX = mData.at("SizeX").get<int>();
	mSizeY = mData.at("SizeY").get<int>();

	mTileset = &GetTileset(mData.at("Tileset"));

	for (const auto& TileData : mData.at("Tiles"))
	{
		auto Pos = TileData.at("Position");
		auto X = Pos[0].get<int>();
		auto Y = Pos[1].get<int>();
		auto Tile = W->ConstructActor<PTile>(X, Y);
		Tile->Data.SubIndexes = TileData.at("SubIndexes");
		Tile->Chunk = this;
		Tile->Data.Tileset = mTileset;
		mTiles.emplace_back(Tile);
	}

#if _EDITOR
	if (const auto EditorGame = GetEditorGame())
	{
		EditorGame->AddChunk(this);
	}
#endif
}

void PChunk::Draw(const PRenderer* Renderer) const
{
	if (!GetSettings()->mDebugDraw)
	{
		return;
	}

	const FRect Dest = GetLocalBounds();

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Select) && (mMouseOver || mSelected))
	{
		Renderer->SetDrawColor(255, 200, 0, 150);
		if (mMouseOver)
		{
			constexpr float ExpandSize = 2.0f;
			Renderer->DrawRectAt(Dest.Expanded(ExpandSize),
								 mPosition - FVector2(ExpandSize, ExpandSize));
		}
		if (mSelected)
		{
			Renderer->DrawFillRectAt(Dest, mPosition);
		}
	}
#endif

	Renderer->SetDrawColor(255, 255, 255, 128);
	Renderer->DrawRectAt(Dest, mPosition);
}

FRect PChunk::GetLocalBounds() const
{
	return { 0, 0, mSizeX * DOUBLE_TILE_SIZE, mSizeY * DOUBLE_TILE_SIZE };
}

FRect PChunk::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, mSizeX * DOUBLE_TILE_SIZE, mSizeY * DOUBLE_TILE_SIZE };
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

	Result["Tileset"] = mTileset->Name;

	auto TileArray = json::array();
	for (const auto& Tile : GetTiles())
	{
		TileArray.push_back(Tile->Serialize());
	}
	Result["Tiles"] = TileArray;
	return Result;
}
