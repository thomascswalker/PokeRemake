#include "Tileset.h"

FRect STile::GetSourceRect()
{
	auto	 X = Index % Tileset->Width;
	auto	 Y = Index / Tileset->Width;
	FVector2 CoordIndex(X, Y);
	return {
		CoordIndex * gTilesetItemSize, { gTilesetItemSize, gTilesetItemSize }
	};
}
