#include "Actor.h"

#include "Core/Logging.h"

FVector2 PActor::GetDrawPosition() const
{
	return mPosition - FVector2(0, QUARTER_TILE_SIZE);
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * TILE_SIZE, Y * TILE_SIZE);
}