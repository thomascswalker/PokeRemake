#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/World.h"

PCharacter::PCharacter()
{
	mBounds = FRect(0, 0, TILE_SIZE, TILE_SIZE);
}

void PCharacter::Start()
{
	bInputAllowed = true;
}

void PCharacter::Tick(float DeltaTime)
{
	if (IsMoving())
	{
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

		if (bCloseEnough)
		{
			mVelocity = FVector2(0, 0);	 // Stop moving when at target position
			mPosition = mTargetPosition; // Snap to target position
		}
		else
		{
			switch (mMovementDirection)
			{
				case MD_Right:
					mPosition.X += PLAYER_SPEED * DeltaTime;
					break;
				case MD_Left:
					mPosition.X -= PLAYER_SPEED * DeltaTime;
					break;
				case MD_Down:
					mPosition.Y += PLAYER_SPEED * DeltaTime;
					break;
				case MD_Up:
					mPosition.Y -= PLAYER_SPEED * DeltaTime;
					break;
				default:
					break; // No movement direction set
			}
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

	Renderer->DrawSpriteAt(PTextureManager::Get(TEXTURE_ASH), mBounds,
						   mPosition - FVector2(TILE_SIZE, HALF_TILE_SIZE), Index);
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
