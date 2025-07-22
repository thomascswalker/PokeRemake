#pragma once

#include "Character.h"
#include "Engine/Components/CameraComponent.h"

#include <bitset>

class PPlayerCharacter : public PCharacter
{

	PCameraComponent* mCameraComponent = nullptr;
	std::bitset<4>	  mInputState; // Left, Right, Down, Up
	bool			  bInputAllowed = true;

public:
	PPlayerCharacter();
	void Tick(float DeltaTime) override;
	bool DebugDraw(const PRenderer* Renderer) const override;
	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;

	PCameraComponent* GetCameraComponent() const { return mCameraComponent; }
};
