#include "Grid.h"

#include "Engine/Engine.h"
#include "Engine/InputManager.h"

#include <cmath>

PGrid::PGrid(const uint32_t InX, const uint32_t InY, const uint32_t InCellSize)
	: mSizeX(InX), mSizeY(InY), mZoomFactor(InCellSize), mOffsetX(0), mOffsetY(0)
{
	if (IInputManager* InputManager = GetInputManager())
	{
		InputManager->KeyDown.AddRaw(this, &PGrid::OnKeyDown);
		InputManager->KeyUp.AddRaw(this, &PGrid::OnKeyUp);
		InputManager->MouseScroll.AddRaw(this, &PGrid::OnMouseScroll);
	}
}

void PGrid::Tick(float DeltaTime)
{
	const float Speed = 0.25f * DeltaTime;
	if (mKeysDown[0]) // Right
	{
		AddOffsetX(-Speed);
	}
	if (mKeysDown[1]) // Left
	{
		AddOffsetX(Speed);
	}
	if (mKeysDown[2]) // Down
	{
		AddOffsetY(-Speed);
	}
	if (mKeysDown[3]) // Up
	{
		AddOffsetY(Speed);
	}
}

void PGrid::Draw(IRenderer* Renderer)
{
	Renderer->SetDrawColor(100, 100, 100, 255);

	const float ScreenWidth = Renderer->GetScreenWidth();
	const float ScreenHeight = Renderer->GetScreenHeight();

	// Compute the origin (center of the screen - half of a cell size in order to
	// center the grid).
	const auto OriginX = ScreenWidth / 2.0f - mZoomFactor / 2.0f;
	const auto OriginY = ScreenHeight / 2.0f - mZoomFactor / 2.0f;

	// Compute the number of steps to draw based on the screen size and zoom factor. Only do this
	// for the width, in turn forcing the grid cells to be square.
	const float Steps = ScreenWidth / mZoomFactor;

	// Compute the render offset given the world offset and zoom factor.
	// This gives the perception of an infinite grid.
	const float OffsetX = std::fmod(mOffsetX, mZoomFactor);
	const float OffsetY = std::fmod(mOffsetY, mZoomFactor);

	float X0, X1, Y0, Y1;

	// Draw vertical lines
	for (int32_t Index = -Steps; Index <= Steps; Index++)
	{
		X0 = OffsetX + OriginX - Index * mZoomFactor;
		X1 = X0;
		Y0 = 0;
		Y1 = ScreenHeight;
		Renderer->DrawLine(X0, Y0, X1, Y1);
	}

	// Draw horizontal lines
	for (int32_t Index = -Steps; Index <= Steps; Index++)
	{
		X0 = 0;
		X1 = ScreenWidth;
		Y0 = OffsetY + OriginY - Index * mZoomFactor;
		Y1 = Y0;
		Renderer->DrawLine(X0, Y0, X1, Y1);
	}
}
void PGrid::OnKeyDown(uint32_t ScanCode)
{
	switch (ScanCode)
	{
		case KB_Right: // Right
			mKeysDown[0] = true;
			break;
		case KB_Left: // Left
			mKeysDown[1] = true;
			break;
		case KB_Down: // Down
			mKeysDown[2] = true;
			break;
		case KB_Up: // Up
			mKeysDown[3] = true;
			break;
		default:
			break;
	}
}
void PGrid::OnKeyUp(uint32_t ScanCode)
{
	switch (ScanCode)
	{
		case KB_Right: // Right
			mKeysDown[0] = false;
			break;
		case KB_Left: // Left
			mKeysDown[1] = false;
			break;
		case KB_Down: // Down
			mKeysDown[2] = false;
			break;
		case KB_Up: // Up
			mKeysDown[3] = false;
			break;
		default:
			break;
	}
}
void PGrid::OnMouseScroll(float Value) { mZoomFactor += Value; }
