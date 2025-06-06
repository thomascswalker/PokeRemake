#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/World.h"

void PCharacter::Start()
{
	bInputAllowed = true;
	mTargetPosition = mPosition; // To prevent immediate movement on start
	mPriority = DP_FOREGROUND;

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
		mDistanceTraveled = 0.0f;	 // Reset distance traveled
		mAnimationCycle = false;	 // Reset animation cycle
	}
	// Keep moving towards the target position
	else
	{
		float Distance = DEFAULT_SPEED * DeltaTime;
		switch (mMovementDirection)
		{
			case MD_Right:
				mPosition.X += Distance;
				break;
			case MD_Left:
				mPosition.X -= Distance;
				break;
			case MD_Down:
				mPosition.Y += Distance;
				break;
			case MD_Up:
				mPosition.Y -= Distance;
				break;
			default:
				break; // No movement direction set
		}
		mDistanceTraveled += Distance;
	}

	// Update the animation cycle
	if (mDistanceTraveled > HALF_TILE_SIZE && !mAnimationCycle)
	{
		mAnimationCycle = true;
	}
}

void PCharacter::Draw(const PRenderer* Renderer) const
{
	int Index = 0;
	switch (mMovementDirection)
	{
		case MD_Right:
			if (IsMoving())
			{
				Index = mAnimationCycle ? SI_WalkRight : SI_IdleRight;
			}
			else
			{
				Index = SI_IdleRight;
			}
			break;
		case MD_Left:
			if (IsMoving())
			{
				Index = mAnimationCycle ? SI_WalkLeft : SI_IdleLeft;
			}
			else
			{
				Index = SI_IdleLeft;
			}
			break;
		case MD_Down:
			if (IsMoving())
			{
				Index = mAnimationCycle ? SI_WalkDownA : SI_WalkDownB;
			}
			else
			{
				Index = SI_IdleDown;
			}
			break;
		case MD_Up:
			if (IsMoving())
			{
				Index = mAnimationCycle ? SI_WalkUpA : SI_WalkUpB;
			}
			else
			{
				Index = SI_IdleUp;
			}
			break;
		default:
			Index = 0; // Default to walk down if no direction is set
			break;
	}

	Renderer->DrawSpriteAt(mTexture, GetLocalBounds(), GetDrawPosition(), Index);
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
	if (!Tile->bWalkable)
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
