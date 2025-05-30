#include "Character.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

void PCharacter::Start()
{
	// Bind input
	if (const auto Input = GetInputManager())
	{
		Input->KeyDown.AddRaw(this, &PCharacter::OnKeyDown);
	}
	else
	{
		LogError("Input manager is not initialized, cannot add key down handler for character");
	}
}

void PCharacter::Tick(float DeltaTime)
{
	const float Speed = 0.1f * DeltaTime;
	if (mKeysDown[0]) // Right
	{
		mPosition.X += Speed * DeltaTime;
	}
	if (mKeysDown[1]) // Left
	{
		mPosition.X -= Speed * DeltaTime;
	}
	if (mKeysDown[2]) // Down
	{
		mPosition.Y += Speed * DeltaTime;
	}
	if (mKeysDown[3]) // Up
	{
		mPosition.Y -= Speed * DeltaTime;
	}
}

void PCharacter::Draw(PRenderer* Renderer)
{
	// TODO: Implement global coordinate system for rendering

	const auto Width = Renderer->GetScreenWidth();
	const auto Height = Renderer->GetScreenHeight();

	auto HalfWidth = Width / 2;
	auto HalfHeight = Height / 2;
	HalfWidth += mPosition.X;
	HalfHeight += mPosition.Y;

	const auto ZoomFactor = Renderer->GetZoomFactor();
	const auto X0 = HalfWidth - ZoomFactor;
	const auto Y0 = HalfHeight - ZoomFactor;
	const auto X1 = HalfWidth + ZoomFactor;
	const auto Y1 = HalfHeight + ZoomFactor;

	FRect Rect(X0, Y0, X1 - X0, Y1 - Y0);
	Renderer->SetDrawColor(255, 128, 0, 255);
	Renderer->DrawFillRect(Rect);
}

void PCharacter::OnKeyDown(uint32_t KeyCode)
{
	switch (KeyCode)
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
void PCharacter::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
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