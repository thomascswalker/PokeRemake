#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Components/CameraComponent.h"

#include <bitset>

class PEditorView : public PActor
{
protected:
	std::bitset<4>	  mInputState;
	PCameraComponent* mCameraComponent = nullptr;

public:
	PEditorView() = default;
	~PEditorView() override = default;
	void Start() override;
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override {}

	void OnKeyDown(uint32_t KeyCode);
	void OnKeyUp(uint32_t KeyCode);
};
