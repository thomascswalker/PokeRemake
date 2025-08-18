// ReSharper disable CppDFAUnreachableCode

#include "Map.h"

#include "Core/Macros.h"
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
	auto Position = Map->GetWorldPosition();
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
		Tileset->ItemSize,            // Width
		Tileset->ItemSize,            // Height
	};
}

FRect STile::GetDestRect() const
{
	return {
		GetPosition(), {TILE_SIZE, TILE_SIZE}
	};
}

void PMap::Start()
{
#if _EDITOR
	if (const auto EditorGame = GetEditorGame())
	{
		EditorGame->AddMap(this);
	}
#endif
}

bool PMap::Draw(const PRenderer* Renderer) const
{
	// Draw each tile
	for (const auto& Tile : mTiles)
	{
		VALIDATE(Tile.Tileset, "Tileset is invalid.");
		VALIDATE(Tile.Tileset->Texture, "Tileset texture is invalid.");
		Renderer->DrawTextureAt(Tile.Tileset->Texture, Tile.GetSourceRect(), Tile.GetDestRect());
	}
	return true;
}

bool PMap::DebugDraw(const PRenderer* Renderer) const
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
		Renderer->DrawLineAt({X * TILE_SIZE, 0}, {X * TILE_SIZE, Max.Y});
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
		Renderer->DrawLineAt({0, Y * TILE_SIZE}, {Max.X, Y * TILE_SIZE});
	}
	//
	// #if _EDITOR
	// 	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Select) && (mMouseOver || mSelected))
	// 	{
	// 		auto Dest = GetWorldBounds();
	// 		Renderer->SetDrawColor(255, 200, 0, 150);
	// 		if (mMouseOver)
	// 		{
	// 			constexpr float ExpandSize = 2.0f;
	// 			Renderer->DrawRectAt(Dest.Expanded(ExpandSize));
	// 		}
	// 		if (mSelected)
	// 		{
	// 			Renderer->DrawFillRectAt(Dest);
	// 		}
	// 	}
	// 	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Tile) && mMouseOver)
	// 	{
	// 		auto EditorGame = GetEditorGame();
	// 		auto BrushSize  = EditorGame->GetBrushSize();
	//
	// 		auto MouseWorldPos = Renderer->GetMouseWorldPosition();
	// 		if (auto Tile = GetTileAtPosition(MouseWorldPos))
	// 		{
	// 			Renderer->SetDrawColor(PColor::Red);
	// 			auto HoverRect = Tile->GetDestRect();
	// 			if (BrushSize == BS_Large)
	// 			{
	// 				HoverRect.W *= 2.0f;
	// 				HoverRect.H *= 2.0f;
	// 			}
	// 			Renderer->DrawRectAt(HoverRect);
	// 		}
	// 	}
	// 	if (Bitmask::Test(GetEditorGame()->GetInputContext(), IC_Actor) && mMouseOver)
	// 	{
	// 		auto MouseWorldPos = Renderer->GetMouseWorldPosition();
	// 		if (auto Tile = GetTileAtPosition(MouseWorldPos))
	// 		{
	// 			Renderer->SetDrawColor(PColor::Red);
	// 			auto HoverRect = Tile->GetDestRect();
	// 			HoverRect.W *= 2.0f;
	// 			HoverRect.H *= 2.0f;
	// 			Renderer->DrawRectAt(HoverRect);
	// 		}
	// 	}
	// #endif

	return true;
}

FRect PMap::GetLocalBounds() const
{
	return {0, 0, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE};
}

FRect PMap::GetWorldBounds() const
{
	return {mPosition.X, mPosition.Y, mSizeX * TILE_SIZE, mSizeY * TILE_SIZE};
}

std::vector<STile*> PMap::GetTiles()
{
	std::vector<STile*> Tiles;
	for (auto& Tile : mTiles)
	{
		Tiles.push_back(&Tile);
	}
	return Tiles;
}

STile* PMap::GetTile(int Index)
{
	if (Index < 0 || Index >= mTiles.size())
	{
		return nullptr;
	}
	return &mTiles[Index];
}

STile* PMap::GetTileAtPosition(const FVector2& Position) const
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

STile* PMap::GetTileUnderMouse() const
{
	return GetTileAtPosition(GetRenderer()->GetMouseWorldPosition());
}

STile* PMap::GetTileAt(int X, int Y) const
{
	if (X < 0 || X >= mSizeX || Y < 0 || Y >= mSizeY)
	{
		return nullptr;
	}
	auto Index        = Y * mSizeX + X;
	const STile* Tile = &mTiles[Index];

	return const_cast<STile*>(Tile);
}

JSON PMap::Serialize() const
{
	JSON Result = PActor::Serialize();
	SAVE_MEMBER_PROPERTY(MapName);
	SAVE_MEMBER_PROPERTY(SizeX);
	SAVE_MEMBER_PROPERTY(SizeY);

	auto TileArray = JSON::array();
	for (const auto& Tile : mTiles)
	{
		JSON TileResult;
		TileResult["Tileset"] = Tile.Tileset->Name;
		TileResult["Index"]   = Tile.Index;
		TileArray.push_back(TileResult);
	}
	Result["Tiles"] = TileArray;
	return Result;
}

void PMap::Deserialize(const JSON& Data)
{
	PActor::Deserialize(Data);
	mDrawPriority = Z_BG;
	mBlocking     = false;

	LOAD_MEMBER_PROPERTY(MapName, std::string);
	LOAD_MEMBER_PROPERTY(SizeX, int32_t);
	LOAD_MEMBER_PROPERTY(SizeY, int32_t);

	auto TileData = Data.at("Tiles");
	for (int32_t I = 0; I < TileData.size(); I++)
	{
		auto Tile    = TileData[I];
		auto X       = I % mSizeX;
		auto Y       = I / mSizeX;
		auto Index   = Tile.at("Index").get<int>();
		auto Tileset = Tile.contains("Tileset") ? GetTileset(Tile.at("Tileset")) : GetDefaultTileset();
		mTiles.push_back({Tileset, this, Index, X, Y});
	}

#if _EDITOR
	auto EditorGame = GetEditorGame();
	EditorGame->SetCurrentMap(this);
#endif
}
