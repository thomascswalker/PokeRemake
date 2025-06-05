#include "PlayerCharacter.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

#define PLAYER_SPEED 1.0f

void PPlayerCharacter::Start()
{
	// Bind input
	if (const auto Input = GetInputManager())
	{
		Input->KeyDown.AddRaw(this, &PPlayerCharacter::OnKeyDown);
		Input->KeyUp.AddRaw(this, &PPlayerCharacter::OnKeyUp);
	}

	mCameraComponent = GetWorld()->ConstructComponent<PCameraComponent>(this);
}

void PPlayerCharacter::Tick(float DeltaTime)
{
	if (!IsMoving() && mInputState.any())
	{
		FVector2 Target;
		if (mInputState[0]) // Right
		{
			Target = { TILE_SIZE, 0 };
		}
		else if (mInputState[1]) // Left
		{
			Target = { -TILE_SIZE, 0 };
		}
		else if (mInputState[2]) // Down
		{
			Target = { 0, TILE_SIZE };
		}
		else if (mInputState[3]) // Up
		{
			Target = { 0, -TILE_SIZE };
		}
		SetRelativeTargetPosition(Target);
		UpdateMovementDirection(Target);
	}

	PCharacter::Tick(DeltaTime);
}
void PPlayerCharacter::Draw(const PRenderer* Renderer) const
{
	PCharacter::Draw(Renderer);

	FRect R = { 0, 0, 8, 8 };
	Renderer->SetDrawColor(0, 255, 0, 255);
	Renderer->DrawFillRectAt(R, mPosition);
}

void PPlayerCharacter::OnKeyDown(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case KB_Right: // Right
		case KB_D:
			mInputState[0] = true;
			break;
		case KB_Left: // Left
		case KB_A:
			mInputState[1] = true;
			break;
		case KB_Down: // Down
		case KB_S:
			mInputState[2] = true;
			break;
		case KB_Up: // Up
		case KB_W:
			mInputState[3] = true;
			break;
		default:
			break;
	}

	if (mInputState.any())
	{
		bInputAllowed = false; // Disable further input until the character stops moving
	}
}

void PPlayerCharacter::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case KB_Right: // Right
		case KB_D:
			mInputState[0] = false;
			break;
		case KB_Left: // Left
		case KB_A:
			mInputState[1] = false;
			break;
		case KB_Down: // Down
		case KB_S:
			mInputState[2] = false;
			break;
		case KB_Up: // Up
		case KB_W:
			mInputState[3] = false;
			break;
		default:
			break;
	}
	if (mInputState.none())
	{
		bInputAllowed = true; // Enable input again when no keys are pressed
	}
}
