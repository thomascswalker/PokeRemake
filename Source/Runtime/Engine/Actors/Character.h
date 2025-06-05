#pragma once

#include "Actor.h"
#include "Grid.h"

enum EMovementDirection
{
	MD_Left,
	MD_Right,
	MD_Up,
	MD_Down
};

class PCharacter : public PActor
{
protected:
	FVector2		   mTargetPosition;
	EMovementDirection mMovementDirection = MD_Down;
	bool			   bInputAllowed = false;

public:
	PCharacter() = default;
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;

	FRect GetLocalBounds() const override;
	FRect GetWorldBounds() const override;

	void SetRelativeTargetPosition(const FVector2& Target);
	bool AtTargetPosition() const { return mPosition == mTargetPosition; }
	bool IsMoving() const { return mTargetPosition != mPosition; }
	void UpdateMovementDirection(const FVector2& Direction);

	PTile* GetCurrentTile() const;
};
