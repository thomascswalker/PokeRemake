#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/World.h"

void PCharacter::Start()
{
	bInputAllowed = true;
	mTargetPosition = mPosition; // To prevent immediate movement on start
	mPriority = DP_FOREGROUND;

	mSprite = PSprite();
	mSprite.SetTexture(PTextureManager::Get(TEXTURE_GARY));
	mSprite.AddAnimation("WalkRight", { SI_WalkRight, SI_IdleRight });
	mSprite.AddAnimation("WalkLeft", { SI_WalkLeft, SI_IdleLeft });
	mSprite.AddAnimation("WalkUp", { SI_WalkUpA, SI_WalkUpB });
	mSprite.AddAnimation("WalkDown", { SI_WalkDownA, SI_WalkDownB });
	mSprite.AddAnimation("IdleRight", { SI_IdleRight });
	mSprite.AddAnimation("IdleLeft", { SI_IdleLeft });
	mSprite.AddAnimation("IdleUp", { SI_IdleUp });
	mSprite.AddAnimation("IdleDown", { SI_IdleDown });
}

void PCharacter::Tick(float DeltaTime)
{
	mSprite.Tick(DeltaTime);

	// If we're not moving, just exit
	if (!IsMoving())
	{
		switch (mMovementDirection)
		{
			case MD_Right:
				mSprite.SetCurrentAnimation("IdleRight");
				break;
			case MD_Left:
				mSprite.SetCurrentAnimation("IdleLeft");
				break;
			case MD_Down:
				mSprite.SetCurrentAnimation("IdleDown");
				break;
			case MD_Up:
				mSprite.SetCurrentAnimation("IdleUp");
				break;
			default:
				break; // No movement direction set
		}
		return;
	}

	// Are we close enough to snap to the target position?
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
		// Kind of arbitrarily divide the distance traveled by 2 to
		// make the character move at a reasonable speed.
		const float Distance = DEFAULT_CHAR_SPEED * DeltaTime * 0.5f;

		// Add distance traveled in the corresponding direction to the current position
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
}

void PCharacter::Draw(const PRenderer* Renderer) const
{
	Renderer->DrawSpriteAt(mSprite.GetTexture(), GetLocalBounds(), GetDrawPosition(),
						   mSprite.GetCurrentAnimation()->GetCurrentIndex());
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

void PCharacter::Move(EMovementDirection Direction)
{
	FVector2 Target;
	switch (Direction) // Right
	{
		case MD_Right:
			{
				Target = { TILE_SIZE, 0 };
				mSprite.SetCurrentAnimation("WalkRight");
				break;
			}
		case MD_Left:
			{
				Target = { -TILE_SIZE, 0 };
				mSprite.SetCurrentAnimation("WalkLeft");
				break;
			}
		case MD_Down:
			{
				Target = { 0, TILE_SIZE };
				mSprite.SetCurrentAnimation("WalkDown");
				break;
			}

		case MD_Up:
			{
				Target = { 0, -TILE_SIZE };
				mSprite.SetCurrentAnimation("WalkUp");
				break;
			}
	}

	SetRelativeTargetPosition(Target);
	UpdateMovementDirection(Target);
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
