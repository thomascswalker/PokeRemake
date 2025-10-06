#include "CharacterMovementComponent.h"

#include "Core/Logging.h"
#include "Engine/MapManager.h"
#include "Engine/World.h"

bool PCharacterMovementComponent::Start()
{
	// Set target position to current position to prevent automatic movement to [0,0]
	// on game startup
	mTargetPosition = mOwner->GetWorldPosition2D();
	mCurrentMap = GMapManager->GetMapAtPosition(mTargetPosition);
	if (!mCurrentMap)
	{
		LogError("No valid maps in the world.");
		return false;
	}
	return true;
}

void PCharacterMovementComponent::Tick(float DeltaTime)
{
	// Are we moving? If not, check if we can start moving
	if (!IsMoving())
	{
		return;
	}

	// Are we close enough to snap to the target position?
	bool CloseEnough = false;
	auto OwnerPosition = mOwner->GetWorldPosition2D();
	switch (mMovementDirection)
	{
		case OR_East:
			CloseEnough = OwnerPosition.X >= mTargetPosition.X;
			break;
		case OR_West:
			CloseEnough = OwnerPosition.X <= mTargetPosition.X;
			break;
		case OR_South:
			CloseEnough = OwnerPosition.Y >= mTargetPosition.Y;
			break;
		case OR_North:
			CloseEnough = OwnerPosition.Y <= mTargetPosition.Y;
			break;
		default:
			break;
	}

	// End movement, we've reached the target position
	if (CloseEnough || OwnerPosition.CloseEnough(mTargetPosition))
	{
		mDistanceTraveled = 0.0f;
		mOwner->SetPosition2D(mTargetPosition);
		MovementEnded.Broadcast(mMovementDirection);
		return;
	}

	// Kind of arbitrarily divide the distance traveled by 2 to
	// make the character move at a reasonable speed.
	const float Distance = DEFAULT_CHAR_SPEED * DeltaTime;

	// Add distance traveled in the corresponding direction to the current position
	switch (mMovementDirection)
	{
		case OR_East:
			{
				OwnerPosition.X += Distance;
				break;
			}
		case OR_West:
			{
				OwnerPosition.X -= Distance;
				break;
			}
		case OR_South:
			{
				OwnerPosition.Y += Distance;
				break;
			}
		case OR_North:
			{
				OwnerPosition.Y -= Distance;
				break;
			}
		default:
			{
				break;
			}
	}
	mOwner->SetPosition2D(OwnerPosition);
	mDistanceTraveled += Distance;
}

bool PCharacterMovementComponent::IsMoving() const
{
	return mOwner->GetWorldPosition2D() != mTargetPosition;
}

bool PCharacterMovementComponent::Move(const FVector2& Velocity)
{
	// Compute the target position
	const auto NewPosition = Velocity + mOwner->GetWorldPosition2D();

	// Convert velocity to a movement direction
	// Allow changing direction even if the character doesn't move
	SetMovementDirection(VectorToDirection(Velocity));

	// If the new position is not within the same map as the current map we're in,
	// set the current map to the map at the new position.
	if (!mCurrentMap->GetWorldBounds().Contains(NewPosition))
	{
		PGameMap* NewMap = GMapManager->GetMapAtPosition(NewPosition);

		// If no map is found at the new position, return false.
		if (!NewMap)
		{
			return false; // No map at the new position
		}
		mCurrentMap = NewMap;
	}

	// Check if the new position is walkable
	const auto Tile = mCurrentMap->GetTileAtPosition(NewPosition);
	if (!Tile || Tile->IsBlocking())
	{
		return false;
	}

	// Check if the tile contains an actor that blocks movement
	const auto Actor = GWorld->GetActorAtPosition(NewPosition);
	if (Actor && Actor->IsBlocking())
	{
		return false;
	}

	// Start movement
	mTargetPosition = NewPosition;
	MovementStarted.Broadcast(mMovementDirection);

	return true;
}

STile* PCharacterMovementComponent::GetCurrentTile() const
{
	if (!mCurrentMap)
	{
		return {};
	}
	return mCurrentMap->GetTileAtPosition(mOwner->GetWorldPosition2D());
}

STile* PCharacterMovementComponent::GetTargetTile() const
{
	auto GameMap = GMapManager->GetMapAtPosition(mTargetPosition);
	if (!GameMap)
	{
		return {};
	}
	return mCurrentMap->GetTileAtPosition(mTargetPosition);
}

void PCharacterMovementComponent::SetCurrentMap(PGameMap* NewMap)
{
	mCurrentMap = NewMap;
}

void PCharacterMovementComponent::SnapToPosition(const FVector2& Position, PGameMap* GameMap, bool Emit)
{
	if (!GameMap)
	{
		GameMap = GMapManager->GetMapAtPosition(mTargetPosition);
		if (!GameMap)
		{
			LogWarning("No map at target position.");
		}
	}
	mCurrentMap = GameMap;
	mOwner->SetPosition2D(Position);
	mTargetPosition = Position;

	if (Emit)
	{
		MovementEnded.Broadcast(mMovementDirection);
	}
}

void PCharacterMovementComponent::SnapToTile(const IVector2& Position)
{
	auto Tile = mCurrentMap->GetTileAt(Position.X, Position.Y);
	if (!Tile)
	{
		LogError("No tile found at: {}", Position.ToString().c_str());
		return;
	}
	auto TilePosition = Tile->GetPosition();
	mOwner->SetPosition2D(TilePosition);
	mTargetPosition = TilePosition;
	MovementEnded.Broadcast(mMovementDirection);
}
