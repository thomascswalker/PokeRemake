#include "PlayerCharacter.h"

#include "Engine/InputManager.h"
#include "Engine/World.h"

#define PLAYER_SPEED 1.0f

void PPlayerCharacter::Start()
{
	const float Size = 32.0f;
	mVertices.push_back({ -Size, -Size });
	mVertices.push_back({ Size, -Size });
	mVertices.push_back({ Size, Size });
	mVertices.push_back({ -Size, Size });
	mIndices = { 0, 1, 2, 0, 2, 3 };

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
	const float Speed = PLAYER_SPEED * 0.1f * DeltaTime;
	if (mInputState[0]) // Right
	{
		mPosition.X += Speed * DeltaTime;
	}
	if (mInputState[1]) // Left
	{
		mPosition.X -= Speed * DeltaTime;
	}
	if (mInputState[2]) // Down
	{
		mPosition.Y += Speed * DeltaTime;
	}
	if (mInputState[3]) // Up
	{
		mPosition.Y -= Speed * DeltaTime;
	}
}
void PPlayerCharacter::Draw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(255, 255, 255, 255);
	Renderer->DrawMesh(mVertices, mIndices, { mPosition.X, mPosition.Y });
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
}
