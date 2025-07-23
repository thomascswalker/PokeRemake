#include "CharacterMovementComponent.h"

#include "Core/Logging.h"
#include "Engine/World.h"

void PCharacterMovementComponent::Start()
{
	// Set target position to current position to prevent automatic movement to [0,0]
	// on game startup
	mTargetPosition = mOwner->GetWorldPosition();
	mCurrentMap = GetWorld()->GetMapAtPosition(mTargetPosition);
	if (!mCurrentMap)
	{
		LogError("No valid maps in the world.");
	}
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
	auto OwnerPosition = mOwner->GetWorldPosition();
	switch (mMovementDirection)
	{
		case MD_Right:
			CloseEnough = OwnerPosition.X >= mTargetPosition.X;
			break;
		case MD_Left:
			CloseEnough = OwnerPosition.X <= mTargetPosition.X;
			break;
		case MD_Down:
			CloseEnough = OwnerPosition.Y >= mTargetPosition.Y;
			break;
		case MD_Up:
			CloseEnough = OwnerPosition.Y <= mTargetPosition.Y;
			break;
	}

	// End movement, we've reached the target position
	if (CloseEnough || OwnerPosition.CloseEnough(mTargetPosition))
	{
		mDistanceTraveled = 0.0f;
		mOwner->SetPosition(mTargetPosition);
		MovementEnded.Broadcast(mMovementDirection);
		return;
	}

	// Kind of arbitrarily divide the distance traveled by 2 to
	// make the character move at a reasonable speed.
	const float Distance = DEFAULT_CHAR_SPEED * DeltaTime;

	// Add distance traveled in the corresponding direction to the current position
	switch (mMovementDirection)
	{
		case MD_Right:
			OwnerPosition.X += Distance;
			break;
		case MD_Left:
			OwnerPosition.X -= Distance;
			break;
		case MD_Down:
			OwnerPosition.Y += Distance;
			break;
		case MD_Up:
			OwnerPosition.Y -= Distance;
			break;
	}
	mOwner->SetPosition(OwnerPosition);
	mDistanceTraveled += Distance;
}

bool PCharacterMovementComponent::IsMoving() const
{
	return mOwner->GetWorldPosition() != mTargetPosition;
}

bool PCharacterMovementComponent::Move(const FVector2& Velocity)
{
	// Compute the target position
	const auto NewPosition = Velocity + mOwner->GetWorldPosition();

	// Convert velocity to a movement direction
	mMovementDirection = VectorToDirection(Velocity);

	// Allow changing direction even if the character doesn't move
	MovementDirectionChanged.Broadcast(mMovementDirection);

	// If the new position is not within the same map as the current map we're in,
	// set the current map to the map at the new position.
	if (!mCurrentMap->GetWorldBounds().Contains(NewPosition))
	{
		PMap* NewMap = GetWorld()->GetMapAtPosition(NewPosition);

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
	const auto Actor = GetWorld()->GetActorAtPosition(NewPosition);
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
	return mCurrentMap->GetTileAtPosition(mOwner->GetWorldPosition());
}

STile* PCharacterMovementComponent::GetTargetTile() const
{
	auto Map = GetWorld()->GetMapAtPosition(mTargetPosition);
	if (!Map)
	{
		return {};
	}
	return mCurrentMap->GetTileAtPosition(mTargetPosition);
}

void PCharacterMovementComponent::SetCurrentMap(PMap* Map)
{
	mCurrentMap = Map;
}

void PCharacterMovementComponent::SnapToPosition(const FVector2& Position, PMap* Map)
{
	LogDebug("Moving character to: {}", Position.ToString().c_str());
	if (!Map)
	{
		Map = GetWorld()->GetMapAtPosition(mTargetPosition);
		if (!Map)
		{
			LogError("No map at target position.");
			return;
		}
	}
	mCurrentMap = Map;
	mOwner->SetPosition(Position);
	mTargetPosition = Position;
	MovementEnded.Broadcast(mMovementDirection);
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
	mOwner->SetPosition(TilePosition);
	mTargetPosition = TilePosition;
	MovementEnded.Broadcast(mMovementDirection);
}