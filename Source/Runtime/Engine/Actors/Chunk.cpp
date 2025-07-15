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

	mTileset = GetTileset(mData.at("Tileset"));

	for (const auto& Tile : mData.at("Tiles"))
	{
		auto Pos = Tile.at("Position");
		auto X = Pos[0].get<int>();
		auto Y = Pos[1].get<int>();
		auto TileActor = W->ConstructActor<PTile>(X, Y);
		auto Index = Tile.at("Index").get<int>();
		TileActor->TilesetIndex = Index;
		TileActor->Chunk = this;
		TileActor->Tileset = mTileset;
		TileActor->SetParent(this);
		mTiles.push_back(TileActor);
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
	Renderer->DrawRectAt(Dest, 2.0f);

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Select) && (mMouseOver || mSelected))
	{
		Renderer->SetDrawColor(255, 200, 0, 150);
		if (mMouseOver)
		{
			constexpr float ExpandSize = 2.0f;
			Renderer->DrawRectAt(Dest.Expanded(ExpandSize));
		}
		if (mSelected)
		{
			Renderer->DrawFillRectAt(Dest);
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

PTile* PChunk::GetBlockAtPosition(const FVector2& Position) const
{
	for (auto& Block : mTiles)
	{
		if (Block->Contains(Position))
		{
			// Return a pointer to the block
			return Block;
		}
	}
	return nullptr; // No tile found at the given position
}

PTile* PChunk::GetBlockAt(int X, int Y)
{
	if (X < 0 || X >= mSizeX || Y < 0 || Y >= mSizeY)
	{
		return nullptr;
	}
	auto Index = Y * mSizeX + X;
	return mTiles.at(Index);
}

std::vector<PTile*> PChunk::GetAdjacentBlocks(const PTile* OriginBlock, int32_t BrushSize) const
{
	std::vector<PTile*> Result = {};
	auto				BlockCenter = OriginBlock->GetCenter();

	for (int32_t Y = 0; Y < BrushSize + 1; Y++)
	{
		for (int32_t X = 0; X < BrushSize + 1; X++)
		{
			FVector2 TargetPosition = BlockCenter + FVector2(X * BLOCK_SIZE, Y * BLOCK_SIZE);
			auto	 Adjacent = GetBlockAtPosition(TargetPosition);
			if (Adjacent && Adjacent != OriginBlock)
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
				Direction.Y = -BLOCK_SIZE;
				break;
			case SDLK_DOWN:
				Direction.Y = BLOCK_SIZE;
				break;
			case SDLK_LEFT:
				Direction.X = -BLOCK_SIZE;
				break;
			case SDLK_RIGHT:
				Direction.X = BLOCK_SIZE;
				break;
			default:
				break;
		}
		AddPosition(Direction);
		Event->Consume();
	}
}
#endif