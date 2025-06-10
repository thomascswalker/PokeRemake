#include "EditorView.h"

#include "Engine/InputManager.h"
#include "Engine/World.h"

void PEditorView::Start()
{
	bBlocking = false;

	if (const auto W = GetWorld())
	{
		W->ConstructComponent<PCameraComponent>(this);
	}

	if (auto Input = GetInputManager())
	{
		Input->KeyDown.AddRaw(this, &PEditorView::OnKeyDown);
	}
}

void PEditorView::OnKeyDown(const uint32_t KeyCode)
{
	const auto CameraSpeed = 50.0f;
	switch (KeyCode)
	{
		case KB_W:
			mPosition += { CameraSpeed, 0 };
			break;
		case KB_S:
			mPosition += { -CameraSpeed, 0 };
			break;
		case KB_A:
			mPosition += { 0, -CameraSpeed };
			break;
		case KB_D:
			mPosition += { 0, CameraSpeed };
			break;
		default:
			break;
	}
}