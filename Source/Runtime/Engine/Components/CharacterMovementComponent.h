#pragma once

#include "Component.h"
#include "Core/Delegate.h"
#include "Engine/Actors/Chunk.h"

enum EMovementDirection
{
	MD_Left,
	MD_Right,
	MD_Up,
	MD_Down
};

inline FVector2 DirectionToVector(EMovementDirection Direction)
{
	switch (Direction)
	{
		case MD_Right:
			return { 1, 0 };
		case MD_Left:
			return { -1, 0 };
		case MD_Down:
			return { 0, 1 };
		case MD_Up:
			return { 0, -1 };
		default:
			return { 0, 0 };
	}
}

inline EMovementDirection VectorToDirection(const FVector2& Direction)
{
	if (Direction.X > 0)
	{
		return MD_Right;
	}
	if (Direction.X < 0)
	{
		return MD_Left;
	}
	if (Direction.Y > 0)
	{
		return MD_Down;
	}
	if (Direction.Y < 0)
	{
		return MD_Up;
	}
	return MD_Down;
}

DECLARE_MULTICAST_DELEGATE(DDestinationReached, const FVector2&);
DECLARE_MULTICAST_DELEGATE(DMovementStarted, EMovementDirection);
DECLARE_MULTICAST_DELEGATE(DMovementEnded, EMovementDirection);
DECLARE_MULTICAST_DELEGATE(DMovementDirectionChanged, EMovementDirection);

class PCharacterMovementComponent : public PComponent
{
	FVector2		   mTargetPosition;
	EMovementDirection mMovementDirection;
	FVector2		   mVelocity;
	float			   mDistanceTraveled = 0.0f;
	PChunk*			   mCurrentChunk = nullptr;

public:
	DDestinationReached		  DestinationReached;
	DMovementStarted		  MovementStarted;
	DMovementEnded			  MovementEnded;
	DMovementDirectionChanged MovementDirectionChanged;

	PCharacterMovementComponent() = default;

	void	 Start() override;
	void	 Tick(float DeltaTime) override;
	bool	 IsMoving() const;
	bool	 Move(const FVector2& Velocity);
	FVector2 GetTargetPosition() const { return mTargetPosition; }
	STile*	 GetCurrentTile() const;
	STile*	 GetTargetTile() const;
};