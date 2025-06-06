#include "PlayerCharacter.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Core/Settings.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

#define PLAYER_SPEED 1.0f

void PPlayerCharacter::Start()
{
	PCharacter::Start();
	// Bind input
	if (const auto Input = GetInputManager())
	{
		Input->KeyDown.AddRaw(this, &PPlayerCharacter::OnKeyDown);
		Input->KeyUp.AddRaw(this, &PPlayerCharacter::OnKeyUp);
	}

	mCameraComponent = GetWorld()->ConstructComponent<PCameraComponent>(this);
	mSprite.SetTexture(PTextureManager::Get(TEXTURE_ASH));
}

void PPlayerCharacter::Tick(float DeltaTime)
{
	// Start movement
	if (!IsMoving() && mInputState.any())
	{
		if (mInputState[0]) // Right
		{
			Move(MD_Right);
		}
		else if (mInputState[1]) // Left
		{
			Move(MD_Left);
		}
		else if (mInputState[2]) // Down
		{
			Move(MD_Down);
		}
		else if (mInputState[3]) // Up
		{
			Move(MD_Up);
		}
	}

	PCharacter::Tick(DeltaTime);
}

void PPlayerCharacter::Draw(const PRenderer* Renderer) const
{
	if (GetSettings()->bDebugDraw)
	{
		// Draw current tile under the character
		if (const auto& Tile = GetCurrentTile())
		{
			Renderer->SetDrawColor(255, 0, 0, 50);
			Renderer->DrawFillRectAt({ 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE }, Tile->GetPosition());
		}
		// Draw target tile
		if (const auto& Tile = GetGrid()->GetTileAtPosition(mTargetPosition))
		{
			Renderer->SetDrawColor(0, 255, 0, 50);
			Renderer->DrawFillRectAt({ 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE }, Tile->GetPosition());
		}
	}
	PCharacter::Draw(Renderer);
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
