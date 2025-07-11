// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "../Settings.h"
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
		Tile->SetParent(this);
		mTiles.emplace_back(Tile);
	}

#if _EDITOR
	if (const auto EditorGame = GetEditorGame())
	{
		EditorGame->AddChunk(this);
	}
#endif
}

void PChunk::DebugDraw(const PRenderer* Renderer) const
{
	const FRect Dest = GetWorldBounds();
	Renderer->SetDrawColor(PColor::Red);
	Renderer->DrawRectAt(Dest, mPosition, 2.0f);

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Select) && (mMouseOver || mSelected))
	{
		Renderer->SetDrawColor(255, 200, 0, 150);
		if (mMouseOver)
		{
			constexpr float ExpandSize = 2.0f;
			Renderer->DrawRectAt(Dest.Expanded(ExpandSize),
								 mPosition - FVector2(ExpandSize * 2, ExpandSize * 2));
		}
		if (mSelected)
		{
			Renderer->DrawFillRectAt(Dest, mPosition);
		}
	}
#endif
}

FRect PChunk::GetLocalBounds() const
{
	return { 0, 0, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE };
}

FRect PChunk::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE };
}

PTile* PChunk::GetTileAtPosition(const FVector2& Position) const
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

PTile* PChunk::GetTileAt(int X, int Y)
{
	if (X < 0 || X >= mSizeX || Y < 0 || Y >= mSizeY)
	{
		return nullptr;
	}
	auto Index = Y * mSizeX + X;
	return mTiles.at(Index);
}

std::vector<PTile*> PChunk::GetAdjacentTiles(const PTile* OriginTile, int32_t BrushSize) const
{
	std::vector<PTile*> Result = {};
	auto				TileCenter = OriginTile->GetCenter();

	for (int32_t Y = 0; Y < BrushSize + 1; Y++)
	{
		for (int32_t X = 0; X < BrushSize + 1; X++)
		{
			FVector2 TargetPosition = TileCenter + FVector2(X * TILE_SIZE, Y * TILE_SIZE);
			auto	 Adjacent = GetTileAtPosition(TargetPosition);
			if (Adjacent && Adjacent != OriginTile)
			{
				Result.push_back(Adjacent);
			}
		}
	}

	return Result;
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

#if _EDITOR
void PChunk::OnKeyUp(SInputEvent* Event)
{
	if (GetEditorGame()->GetCurrentChunk() == this && mSelected)
	{
		FVector2 Direction;
		switch (Event->KeyUp)
		{
			case SDLK_UP:
				Direction.Y = -TILE_SIZE;
				break;
			case SDLK_DOWN:
				Direction.Y = TILE_SIZE;
				break;
			case SDLK_LEFT:
				Direction.X = -TILE_SIZE;
				break;
			case SDLK_RIGHT:
				Direction.X = TILE_SIZE;
				break;
			default:
				break;
		}
		AddPosition(Direction);
		Event->Consume();
	}
}
#endif