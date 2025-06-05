#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/World.h"

void PCharacter::Start()
{
	bInputAllowed = true;
	mTargetPosition = mPosition; // To prevent immediate movement on start

	mTexture = PTextureManager::Get(TEXTURE_GARY);
}

void PCharacter::Tick(float DeltaTime)
{
	if (!IsMoving())
	{
		return;
	}
	bool bCloseEnough = false;
	switch (mMovementDirection)
	{
		case MD_Right:
			bCloseEnough = mPosition.X >= mTargetPosition.X;
			break;
		case MD_Left:
			bCloseEnough = mPosition.X <= mTargetPosition.X;
			break;
		case MD_Down:
			bCloseEnough = mPosition.Y >= mTargetPosition.Y;
			break;
		case MD_Up:
			bCloseEnough = mPosition.Y <= mTargetPosition.Y;
			break;
		default:
			break;
	}

	// End movement
	if (bCloseEnough || mPosition.CloseEnough(mTargetPosition))
	{
		mPosition = mTargetPosition; // Snap to target position
	}
	// Keep moving towards the target position
	else
	{
		switch (mMovementDirection)
		{
			case MD_Right:
				mPosition.X += DEFAULT_SPEED * DeltaTime;
				break;
			case MD_Left:
				mPosition.X -= DEFAULT_SPEED * DeltaTime;
				break;
			case MD_Down:
				mPosition.Y += DEFAULT_SPEED * DeltaTime;
				break;
			case MD_Up:
				mPosition.Y -= DEFAULT_SPEED * DeltaTime;
				break;
			default:
				break; // No movement direction set
		}
	}
}

void PCharacter::Draw(const PRenderer* Renderer) const
{
	int Index = 0;
	switch (mMovementDirection)
	{
		case MD_Right:
			Index = 9; // Walk right
			break;
		case MD_Left:
			Index = 7; // Walk left
			break;
		case MD_Down:
			Index = 0; // Walk down
			break;
		case MD_Up:
			Index = 3; // Walk up
			break;
		default:
			Index = 0; // Default to walk down if no direction is set
			break;
	}

	Renderer->DrawSpriteAt(mTexture, GetLocalBounds(), mPosition, Index);
}

FRect PCharacter::GetLocalBounds() const
{
	return { 0, 0, TILE_SIZE, TILE_SIZE };
}

FRect PCharacter::GetWorldBounds() const
{
	return { mPosition.X, mPosition.Y, TILE_SIZE, TILE_SIZE };
}

void PCharacter::SetRelativeTargetPosition(const FVector2& Target)
{
	const auto TempTargetPosition = Target + mPosition;
	const auto Tile = GetGrid()->GetTileAtPosition(TempTargetPosition);
	if (!Tile)
	{
		return;
	}
	const auto Actor = Tile->GetActor();
	if (Actor && Actor->IsBlocking())
	{
		return;
	}
	mTargetPosition = Target + mPosition;
}
void PCharacter::UpdateMovementDirection(const FVector2& Direction)
{
	if (Direction.X > 0)
	{
		mMovementDirection = MD_Right;
	}
	else if (Direction.X < 0)
	{
		mMovementDirection = MD_Left;
	}
	else if (Direction.Y > 0)
	{
		mMovementDirection = MD_Down;
	}
	else if (Direction.Y < 0)
	{
		mMovementDirection = MD_Up;
	}
}

PTile* PCharacter::GetCurrentTile() const
{
	return GetGrid()->GetTileAtPosition(mPosition);
}
