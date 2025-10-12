#pragma once

#include <bitset>

#include "Engine/Components/CameraComponent.h"

#include "Character.h"

class PPlayerCharacter : public PCharacter, public IDrawable
{
	PCameraComponent* mCameraComponent = nullptr;
	std::bitset<4>	  mInputState; // Left, Right, Down, Up
	bool			  mInputAllowed = true;

public:
	PPlayerCharacter();
	void Tick(float DeltaTime) override;
	bool DebugDraw(const PRenderer* Renderer) const override;

	float GetDepth() const override
	{
		return Drawing::Z_Player;
	}

	bool CanMove() const;
	void SetCanMove(bool State);
	bool TryMove(EDPad Direction);
	bool TryStop(EDPad Direction);
	void Interact();

	PCameraComponent* GetCameraComponent() const
	{
		return mCameraComponent;
	}
	PSpriteComponent* GetSpriteComponent() const { return mSpriteComponent; }
};
