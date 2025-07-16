#include "Tileset.h"

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
