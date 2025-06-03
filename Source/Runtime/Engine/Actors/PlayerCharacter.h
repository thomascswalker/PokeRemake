#pragma once

#include "Character.h"
#include "Engine/Components/CameraComponent.h"

#include <bitset>

class PPlayerCharacter : public PCharacter
{
	std::bitset<4>	  mInputState;
	PCameraComponent* mCameraComponent = nullptr;

public:
	void OnKeyDown(uint32_t KeyCode);
	void OnKeyUp(uint32_t KeyCode);

	void Start() override;
	void Tick(float DeltaTime) override;

	PCameraComponent* GetCameraComponent() const { return mCameraComponent; }
};
