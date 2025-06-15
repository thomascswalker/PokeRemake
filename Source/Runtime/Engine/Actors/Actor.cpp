#include "Actor.h"

#include "Core/Logging.h"
#include "Engine/ClassRegistry.h"

void PActor::Tick(float DeltaTime)
{
#if _EDITOR
	auto	   R = GetRenderer();
	const auto MousePosition = R->GetMousePosition();
	FVector2   ScreenPosition;
	R->WorldToScreen(GetPosition(), &ScreenPosition);

	FRect ScreenRect = { ScreenPosition.X, ScreenPosition.Y, HALF_TILE_SIZE, HALF_TILE_SIZE };
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
#endif
}

FVector2 PActor::GetDrawPosition() const
{
	return mPosition - FVector2(0, CHARACTER_OFFSET);
}

void PActor::MoveToTile(int32_t X, int32_t Y)
{
	mPosition = FVector2(X * TILE_SIZE, Y * TILE_SIZE);
}