#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Components/CameraComponent.h"

#include <bitset>

class PEditorView : public PActor
{
protected:
	std::bitset<4> mInputState;
	PCameraComponent* mCameraComponent = nullptr;

public:
	PEditorView();
	~PEditorView() override = default;

	void Start() override;
	void Tick(float DeltaTime) override;

	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;
	void OnMouseEvent(SInputEvent* Event) override;
};
