#include "EditorView.h"

#include "Core/Logging.h"
#include "Engine/Input.h"
#include "Engine/World.h"

PEditorView::PEditorView()
{
	mSerializable = false;
	mBlocking     = false;
	mSelectable   = false;

	const auto W     = GWorld;
	mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
}

void PEditorView::Tick(float DeltaTime)
{
	FVector2 Destination;
	auto View               = mCameraComponent->GetCameraView();
	const float CameraSpeed = DeltaTime / View->GetZoom(); // Adjust camera speed based on zoom level

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

	if (Destination)
	{
		AddPosition2D(Destination);
	}
}

void PEditorView::OnKeyDown(SInputEvent* Event)
{
	if (IsInputContext(IC_Text))
	{
		return;
	}
	switch (Event->KeyDown)
	{
	case SDLK_W: mInputState[0] = true;
		// Destination = { 0, -CameraSpeed };
		break;
	case SDLK_S: mInputState[1] = true;
		// Destination = { 0, CameraSpeed };
		break;
	case SDLK_A: mInputState[2] = true;
		// Destination = { -CameraSpeed, 0 };
		break;
	case SDLK_D: mInputState[3] = true;
		// Destination = { CameraSpeed, 0 };
		break;
	default: break;
	}
	Event->Consume();
}

void PEditorView::OnKeyUp(SInputEvent* Event)
{
	if (IsInputContext(IC_Text))
	{
		return;
	}
	switch (Event->KeyUp)
	{
	case SDLK_W: mInputState[0] = false;
		break;
	case SDLK_S: mInputState[1] = false;
		break;
	case SDLK_A: mInputState[2] = false;
		break;
	case SDLK_D: mInputState[3] = false;
		break;
	default: break;
	}
	Event->Consume();
}

void PEditorView::OnMouseEvent(SInputEvent* Event)
{
	switch (Event->Type)
	{
	case IET_MouseScroll: if (mCameraComponent)
		{
			mCameraComponent->GetCameraView()->AddZoom(Event->MouseScroll);
			Event->Consume();
		}
		break;
	default: break;
	}
}
