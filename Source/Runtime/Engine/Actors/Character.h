#pragma once

#include "Actor.h"
#include "Chunk.h"
#include "Engine/Components/CharacterMovementComponent.h"

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
	PCharacterMovementComponent* mMovementComponent;

	FVector2		   mTargetPosition;
	EMovementDirection mMovementDirection = MD_Down;
	bool			   bInputAllowed = false;

	bool  mAnimationCycle = false;
	float mDistanceTraveled = 0.0f;

public:
	PCharacter() = default;
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;

	FRect GetLocalBounds() const override;
	FRect GetWorldBounds() const override;

	void OnMovementStarted(EMovementDirection Direction);
	void OnMovementEnded(EMovementDirection Direction);
};
