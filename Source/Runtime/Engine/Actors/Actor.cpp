#include "Actor.h"

#include "Engine/CameraView.h"

void PActor::Tick(float DeltaTime)
{
	UpdateMouseState();
}

void PActor::UpdateMouseState()
{
	const auto Renderer = GetRenderer();
	const auto MousePosition = Renderer->GetMousePosition();
	FVector2   ScreenPosition;
	Renderer->WorldToScreen(GetPosition(), &ScreenPosition);

	auto  CameraView = GetCameraView();
	auto  Rect = GetWorldBounds();
	FRect ScreenRect = {
		ScreenPosition.X,
		ScreenPosition.Y,
		Rect.W * CameraView->GetZoom(),
		Rect.H * CameraView->GetZoom(),
	};
	const bool NewMouseState = ScreenRect.Contains(MousePosition);

	if (!bMouseOver && NewMouseState)
	{
		HoverBegin.Broadcast();
		bMouseOver = true;
	}
	else if (bMouseOver && !NewMouseState)
	{
		HoverEnd.Broadcast();
		bMouseOver = false;
	}

	bool NewClickState = Renderer->GetMouseLeftDown();
	if (!bMouseDown && NewClickState && bMouseOver)
	{
		bMouseDown = true;
	}
	else if (bMouseDown && !NewClickState && bMouseOver)
	{
		bMouseDown = false;
		Clicked.Broadcast(this);
	}
}

FVector2 PActor::GetDrawPosition() const
{
	return mPosition - FVector2(0, CHARACTER_OFFSET);
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * TILE_SIZE, Y * TILE_SIZE);
}
