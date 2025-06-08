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
	void Start() override;
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;
	void OnKeyDown(uint32_t KeyCode);
	void OnKeyUp(uint32_t KeyCode);

	PCameraComponent* GetCameraComponent() const { return mCameraComponent; }
};
