#pragma once

#include "Component.h"
#include "Core/Common.h"
#include "Core/Delegate.h"
#include "Engine/Actors/Map.h"

inline FVector2 DirectionToVector(EOrientation Direction)
{
	switch (Direction)
	{
	case OR_East: return {1, 0};
	case OR_West: return {-1, 0};
	case OR_South: return {0, 1};
	case OR_North: return {0, -1};
	default: return {0, 0};
	}
}

inline EOrientation VectorToDirection(const FVector2& Direction)
{
	if (Direction.X > 0)
	{
		return OR_East;
	}
	if (Direction.X < 0)
	{
		return OR_West;
	}
	if (Direction.Y > 0)
	{
		return OR_South;
	}
	if (Direction.Y < 0)
	{
		return OR_North;
	}
	return OR_South;
}

DECLARE_MULTICAST_DELEGATE(DDestinationReached, const FVector2&);
DECLARE_MULTICAST_DELEGATE(DMovementStarted, EOrientation);
DECLARE_MULTICAST_DELEGATE(DMovementEnded, EOrientation);
DECLARE_MULTICAST_DELEGATE(DMovementDirectionChanged, EOrientation);

class PCharacterMovementComponent : public PComponent
{
	FVector2 mTargetPosition;
	EOrientation mMovementDirection;
	FVector2 mVelocity;
	float mDistanceTraveled = 0.0f;
	PMap* mCurrentMap       = nullptr;

public:
	DDestinationReached DestinationReached;
	DMovementStarted MovementStarted;
	DMovementEnded MovementEnded;
	DMovementDirectionChanged MovementDirectionChanged;

	PCharacterMovementComponent() = default;

	void Start() override;
	void Tick(float DeltaTime) override;
	bool IsMoving() const;
	bool Move(const FVector2& Velocity);

	FVector2 GetTargetPosition() const
	{
		return mTargetPosition;
	}

	STile* GetCurrentTile() const;
	STile* GetTargetTile() const;
	void SetCurrentMap(PMap* Map);

	void SnapToPosition(const FVector2& Position, PMap* Map = nullptr);
	void SnapToTile(const IVector2& Position);

	EOrientation GetMovementDirection()
	{
		return mMovementDirection;
	}

	void SetMovementDirection(EOrientation Orientation)
	{
		if (Orientation == OR_Same)
		{
			return;
		}
		mMovementDirection = Orientation;
	}
};
