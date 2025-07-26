#pragma once

#include "Engine/Components/CharacterMovementComponent.h"

#include "Actor.h"

#include "Engine/Components/SpriteComponent.h"

enum ESpriteIndex
{
	SI_WalkDownA,
	SI_IdleDown,
	SI_WalkDownB,
	SI_WalkUpA,
	SI_IdleUp,
	SI_WalkUpB,
	SI_IdleLeft,
	SI_WalkLeft,
	SI_IdleRight,
	SI_WalkRight
};

class PCharacter : public PActor
{
protected:
	PCharacterMovementComponent* mMovementComponent = nullptr;
	PSpriteComponent* mSpriteComponent              = nullptr;

	FVector2 mTargetPosition;
	EOrientation mMovementDirection = OR_South;
	bool bInputAllowed              = false;

	bool mAnimationCycle    = false;
	float mDistanceTraveled = 0.0f;

public:
	PCharacter();
	~PCharacter() override = default;

	FRect GetLocalBounds() const override;
	FRect GetWorldBounds() const override;

	void OnMovementStarted(EOrientation Direction);
	void OnMovementEnded(EOrientation Direction);

	PCharacterMovementComponent* GetMovementComponent()
	{
		return mMovementComponent;
	}

	PSpriteComponent* GetSpriteComponent()
	{
		return mSpriteComponent;
	}
};
