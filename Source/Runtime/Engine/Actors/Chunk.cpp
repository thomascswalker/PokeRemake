// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "Engine/ClassRegistry.h"
#include "Engine/World.h"
#if _EDITOR
#include "../../../Editor/EditorGame.h"
#endif

PChunk::PChunk(const json& JsonData)
{
	LogDebug("Constructing Chunk from JSON data");
	mPriority = DP_BACKGROUND;
	bBlocking = false;
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
	mTextureName = mData.value("Texture", std::string());
	PTexture* T = PTextureManager::Load(mTextureName);
	if (!mTextureName.empty())
	{
		if (!T)
		{
			LogWarning("Texture {} not found.", mTextureName.c_str());
		}
		else
		{
			mSprite.SetTexture(T);
		}
	}

	auto W = GetWorld();

	auto Position = mData.at("Position");
	mPosition = FVector2(Position[0].get<float>(), Position[1].get<float>());

	mSizeX = mData.at("SizeX").get<int>();
	mSizeY = mData.at("SizeY").get<int>();

	for (const auto& TileData : mData.at("Tiles"))
	{
		auto Pos = TileData.at("Position");
		auto X = Pos[0].get<int>();
		auto Y = Pos[1].get<int>();
		auto Tile = W->ConstructActor<PTile>(X, Y);
		Tile->Type = static_cast<ETileType>(TileData.at("Type").get<int>());
		Tile->Chunk = this;
		Tile->Texture = T;
		mTiles.emplace_back(Tile);
	}

#if _EDITOR
	if (auto EditorGame = dynamic_cast<PEditorGame*>(GetGame()))
	{
		EditorGame->AddChunk(this);
	}
#endif
}

void PChunk::Draw(const PRenderer* Renderer) const
{
	const FRect Dest = GetLocalBounds();

#if _EDITOR
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Select) && (bMouseOver || bSelected))
	{
		Renderer->SetDrawColor(255, 200, 0, 150);
		if (bMouseOver)
		{
			constexpr float ExpandSize = 2.0f;
			Renderer->DrawRectAt(Dest.Expanded(ExpandSize),
								 mPosition - FVector2(ExpandSize, ExpandSize));
		}
		if (bSelected)
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
	return { 0, 0, mSizeX * HALF_TILE_SIZE, mSizeY * HALF_TILE_SIZE };
}

FRect PChunk::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, mSizeX * HALF_TILE_SIZE, mSizeY * HALF_TILE_SIZE };
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
