#include "CharacterMovementComponent.h"

void PCharacterMovementComponent::Start()
{
	// Set target position to current position to prevent automatic movement to [0,0]
	// on game startup
	mTargetPosition = mOwner->GetPosition();
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

bool PCharacterMovementComponent::SetTargetLocation(const FVector2& Target)
{
	// If a movement is requested, but we're already moving or movement is not allowed,
	// return early.
	const auto NewPosition = Target + mOwner->GetPosition();
	mMovementDirection = VectorToDirection(Target);
	MovementDirectionChanged.Broadcast(mMovementDirection);
	const auto Tile = GetChunk()->GetTileAtPosition(NewPosition);
	if (!Tile || !Tile->IsWalkable())
	{
		return false;
	}

	const auto Actor = Tile->GetActor();
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
	return GetChunk()->GetTileAtPosition(mOwner->GetPosition());
}