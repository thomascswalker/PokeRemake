#include "Actor.h"

#include "Engine/CameraView.h"

void PActor::Tick(float DeltaTime)
{
	UpdateMouseState();
}

void PActor::UpdateMouseState()
{
	auto	   R = GetRenderer();
	const auto MousePosition = R->GetMousePosition();
	FVector2   ScreenPosition;
	R->WorldToScreen(GetPosition(), &ScreenPosition);

	auto  CameraView = GetCameraView();
	FRect ScreenRect = { ScreenPosition.X, ScreenPosition.Y, HALF_TILE_SIZE * CameraView->GetZoom(),
						 HALF_TILE_SIZE * CameraView->GetZoom() };
	bool  NewMouseState = ScreenRect.Contains(MousePosition);
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

	bool NewClickState = R->GetMouseLeftDown();
	if (!bMouseDown && NewClickState && bMouseOver)
	{
		bMouseDown = true;
	}
	else if (bMouseDown && !NewClickState)
	{
		Clicked.Broadcast(this);
		bMouseDown = false;
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