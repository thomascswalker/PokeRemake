#include "CharacterMovementComponent.h"

#include "Core/Logging.h"
#include "Engine/World.h"

void PCharacterMovementComponent::Start()
{
	// Set target position to current position to prevent automatic movement to [0,0]
	// on game startup
	mTargetPosition = mOwner->GetPosition();
	mCurrentChunk = GetWorld()->GetChunkAtPosition(mTargetPosition);
	if (!mCurrentChunk)
	{
		LogError("No valid chunks in the world.");
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
	auto OwnerPosition = mOwner->GetPosition();
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
	return mOwner->GetPosition() != mTargetPosition;
}

bool PCharacterMovementComponent::Move(const FVector2& Velocity)
{
	// Compute the target position
	const auto NewPosition = Velocity + mOwner->GetPosition();
	// Convert velocity to a movement direction
	mMovementDirection = VectorToDirection(Velocity);
	// Allow changing direction even if the character doesn't move
	MovementDirectionChanged.Broadcast(mMovementDirection);

	// If the new position is not within the same chunk as the current chunk we're in,
	// set the current chunk to the chunk at the new position.
	if (!mCurrentChunk->GetWorldBounds().Contains(NewPosition))
	{
		PChunk* NewChunk = GetWorld()->GetChunkAtPosition(NewPosition);

		// If no chunk is found at the new position, return false.
		if (!NewChunk)
		{
			LogError("No chunk found at position: {}", NewPosition.ToString().c_str());
			return false; // No chunk at the new position
		}
		mCurrentChunk = NewChunk;
	}

	// Check if the new position is walkable
	const auto Tile = mCurrentChunk->GetTileAtPosition(NewPosition);
	if (!Tile || !Tile->IsWalkable())
	{
		return false;
	}

	// Check if the tile contains an actor that blocks movement
	const auto Actor = GetWorld()->GetCharacterAtPosition(NewPosition);
	if (Actor && Actor->IsBlocking())
	{
		return false;
	}

	// Start movement
	mTargetPosition = NewPosition;
	MovementStarted.Broadcast(mMovementDirection);

	return true;
}

PTile* PCharacterMovementComponent::GetCurrentTile() const
{
	if (!mCurrentChunk)
	{
		return nullptr;
	}
	return mCurrentChunk->GetTileAtPosition(mOwner->GetPosition());
}

PTile* PCharacterMovementComponent::GetTargetTile() const
{
	auto Chunk = GetWorld()->GetChunkAtPosition(mTargetPosition);
	if (!Chunk)
	{
		return nullptr;
	}
	return Chunk->GetTileAtPosition(mTargetPosition);
}