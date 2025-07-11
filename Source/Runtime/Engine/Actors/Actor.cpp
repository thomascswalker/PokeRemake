#include "Actor.h"

#include "Engine/CameraView.h"
#include "Engine/Components/Component.h"

void PActor::OnMouseEvent(SInputEvent* Event)
{
	const auto Renderer = GetRenderer();
	mMousePosition = Event->MousePosition;
	FVector2 ScreenPosition;
	Renderer->WorldToScreen(GetPosition(), &ScreenPosition);

	auto  CameraView = GetCameraView();
	auto  Rect = GetWorldBounds();
	FRect ScreenRect = {
		ScreenPosition.X,
		ScreenPosition.Y,
		Rect.W * CameraView->GetZoom(),
		Rect.H * CameraView->GetZoom(),
	};
	const bool NewMouseState = ScreenRect.Contains(mMousePosition);

	if (!mMouseOver && NewMouseState)
	{
		HoverBegin.Broadcast();
		mMouseOver = true;
	}
	else if (mMouseOver && !NewMouseState)
	{
		HoverEnd.Broadcast();
		mMouseOver = false;
	}

	bool NewClickState = Renderer->GetMouseLeftDown();
	if (!mMouseDown && NewClickState && mMouseOver)
	{
		mMouseDown = true;
	}
	else if (mMouseDown && !NewClickState && mMouseOver)
	{
		mMouseDown = false;
		Clicked.Broadcast(this);
		Event->Consume();
	}
}

FVector2 PActor::GetDrawPosition() const
{
	return mPosition - FVector2(0, CHARACTER_OFFSET);
}

void PActor::AddComponent(PComponent* Component)
{
	Component->SetOwner(this);
	mComponents.push_back(Component);
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * DOUBLE_TILE_SIZE, Y * DOUBLE_TILE_SIZE);
}
