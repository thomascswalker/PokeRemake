// ReSharper disable CppDFAUnreachableCode

#include "Chunk.h"

#include "../Settings.h"
#include "Engine/ClassRegistry.h"
#include "Engine/World.h"
#if _EDITOR
#include "../../../Editor/EditorGame.h"
#endif

bool STile::IsBlocking() const
{
	return Containers::Contains(Tileset->Blocking, Index);
}

FVector2 STile::GetPosition() const
{
	auto Position = Chunk->GetWorldPosition();
	Position.X += X * TILE_SIZE;
	Position.Y += Y * TILE_SIZE;
	return Position;
}

FRect STile::GetSourceRect() const
{
	float TilesetX = Index % Tileset->Width;
	float TilesetY = Index / Tileset->Width;
	return {
		TilesetX * Tileset->ItemSize, // X
		TilesetY * Tileset->ItemSize, // Y
		Tileset->ItemSize,			  // Width
		Tileset->ItemSize,			  // Height
	};
}

FRect STile::GetDestRect() const
{
	return {
		GetPosition(), { TILE_SIZE, TILE_SIZE }
	};
}

PChunk::PChunk(const json& JsonData)
{
	LogDebug("Constructing Chunk from JSON data");
	mRenderPriority = DP_BACKGROUND;
	mBlocking = false;
	mData = JsonData;
}

void PChunk::Start()
{
	auto Position = mData.at("Position");
	mPosition = FVector2(Position[0].get<float>(), Position[1].get<float>());

	mSizeX = mData.at("SizeX").get<int>();
	mSizeY = mData.at("SizeY").get<int>();

	mTileset = GetTileset(mData.at("Tileset"));

	for (const auto& Tile : mData.at("Tiles"))
	{
		auto X = Tile.at("X").get<int>();
		auto Y = Tile.at("Y").get<int>();
		auto Index = Tile.at("Index").get<int>();
		mTiles.push_back({ mTileset, this, Index, X, Y });
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
	// Draw each tile
	for (const auto& Tile : mTiles)
	{
		Renderer->DrawTextureAt(mTileset->Texture, Tile.GetSourceRect(), Tile.GetDestRect());
	}
}

void PChunk::DebugDraw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(255, 0, 0, 128);
	for (const auto& Tile : mTiles)
	{
		if (Tile.IsBlocking())
		{
			Renderer->DrawFillRectAt(Tile.GetDestRect());
		}
	}

	Renderer->SetDrawColor(200, 200, 200, 255);
	FVector2 Max = GetWorldBounds().Max();
	for (int X = 0; X < mSizeX; X++)
	{
		// Draw tile line
		if (X % 2)
		{
			Renderer->SetDrawColor(200, 200, 200, 255);
		}
		// Draw block line
		else
		{
			Renderer->SetDrawColor(0, 0, 0, 255);
		}
		Renderer->DrawLineAt({ X * TILE_SIZE, 0 }, { X * TILE_SIZE, Max.Y });
	}
	for (int Y = 0; Y < mSizeY; Y++)
	{
		// Draw tile line
		if (Y % 2)
		{
			Renderer->SetDrawColor(200, 200, 200, 255);
		}
		// Draw block line
		else
		{
			Renderer->SetDrawColor(0, 0, 0, 255);
		}
		Renderer->DrawLineAt({ 0, Y * TILE_SIZE }, { Max.X, Y * TILE_SIZE });
	}

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
	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Tile) && mMouseOver)
	{
		auto MouseWorldPos = Renderer->GetMouseWorldPosition();
		auto Tile = GetTileAtPosition(MouseWorldPos);
		if (Tile)
		{
			Renderer->SetDrawColor(PColor::Red);
			Renderer->DrawRectAt(Tile->GetDestRect());
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

STile* PChunk::GetTile(int Index)
{
	if (Index < 0 || Index >= mTiles.size())
	{
		return nullptr;
	}
	return &mTiles[Index];
}

STile* PChunk::GetTileAtPosition(const FVector2& Position) const
{
	for (auto& Tile : mTiles)
	{
		if (Tile.GetDestRect().Contains(Position))
		{
			// Return a pointer to the tile
			return const_cast<STile*>(&Tile);
		}
	}
	return nullptr; // No tile found at the given position
}

STile* PChunk::GetTileAt(int X, int Y) const
{
	if (X < 0 || X >= mSizeX || Y < 0 || Y >= mSizeY)
	{
		return nullptr;
	}
	auto		 Index = Y * mSizeX + X;
	const STile* Tile = &mTiles[Index];

	return const_cast<STile*>(Tile);
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
	for (const auto& Tile : mTiles)
	{
		json TileResult;
		TileResult["X"] = Tile.X;
		TileResult["Y"] = Tile.Y;
		TileResult["Index"] = Tile.Index;
		TileArray.push_back(TileResult);
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