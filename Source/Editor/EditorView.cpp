#include "EditorView.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

void PEditorView::Start()
{
	bBlocking = false;

	if (const auto W = GetWorld())
	{
		LogDebug("Constructing camera component for PEditorView");
		mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
		if (!mCameraComponent)
		{
			LogError("Failed to create camera component for PEditorView");
		}
		else
		{
			LogDebug("Created camera component for PEditorView");
		}
	}

	if (auto Input = GetInputManager())
	{
		Input->KeyDown.AddRaw(this, &PEditorView::OnKeyDown);
		Input->KeyUp.AddRaw(this, &PEditorView::OnKeyUp);
		Input->MouseScroll.AddRaw(this, &PEditorView::OnMouseScroll);
		LogDebug("Bound input events for PEditorView");
	}
	else
	{
		LogError("PEditorView::Start: InputManager is null");
	}
}

void PEditorView::Tick(float DeltaTime)
{
	FVector2	Destination;
	auto		View = GetCameraView();
	const float CameraSpeed =
		DeltaTime / View->GetZoom(); // Adjust camera speed based on zoom level

	if (mInputState[0]) // W
	{
		Destination = FVector2(0, -CameraSpeed);
	}
	if (mInputState[1]) // S
	{
		Destination = FVector2(0, CameraSpeed);
	}
	if (mInputState[2]) // A
	{
		Destination = FVector2(-CameraSpeed, 0);
	}
	if (mInputState[3]) // D
	{
		Destination = FVector2(CameraSpeed, 0);
	}

	AddPosition(Destination);
}

void PEditorView::OnKeyDown(const uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case SDLK_W:
			mInputState[0] = true;
			// Destination = { 0, -CameraSpeed };
			break;
		case SDLK_S:
			mInputState[1] = true;
			// Destination = { 0, CameraSpeed };
			break;
		case SDLK_A:
			mInputState[2] = true;
			// Destination = { -CameraSpeed, 0 };
			break;
		case SDLK_D:
			mInputState[3] = true;
			// Destination = { CameraSpeed, 0 };
			break;
		default:
			break;
	}
}
void PEditorView::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case SDLK_W:
			mInputState[0] = false;
			break;
		case SDLK_S:
			mInputState[1] = false;
			break;
		case SDLK_A:
			mInputState[2] = false;
			break;
		case SDLK_D:
			mInputState[3] = false;
			break;
		default:
			break;
	}
}