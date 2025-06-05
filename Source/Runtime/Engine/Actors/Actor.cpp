#include "Actor.h"

#include "Core/Logging.h"

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * TILE_SIZE, Y * TILE_SIZE);
}