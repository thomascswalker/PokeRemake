#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Components/CameraComponent.h"

class PEditorView : public PActor
{
protected:
	PCameraComponent* mCameraComponent = nullptr;

public:
	PEditorView() = default;
	~PEditorView() override = default;
	void Start() override;
	void Tick(float DeltaTime) override {}
	void Draw(const PRenderer* Renderer) const override {}
	void OnKeyDown(uint32_t KeyCode);
};
