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
	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;

	float GetDepth() const override
	{
		return Drawing::Z_Player;
	}

	bool CanMove() const;
	void Interact();

	PCameraComponent* GetCameraComponent() const
	{
		return mCameraComponent;
	}
};
