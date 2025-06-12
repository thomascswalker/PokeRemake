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
	PCharacter::Tick(DeltaTime);
	if (!mMovementComponent->IsMoving() && mInputState.any())
	{
		if (mInputState[0])
		{
			mMovementComponent->Move({ TILE_SIZE, 0 });
		}
		else if (mInputState[1])
		{
			mMovementComponent->Move({ -TILE_SIZE, 0 });
		}
		else if (mInputState[2])
		{
			mMovementComponent->Move({ 0, TILE_SIZE });
		}
		else if (mInputState[3])
		{
			mMovementComponent->Move({ 0, -TILE_SIZE });
		}
	}
}

void PPlayerCharacter::Draw(const PRenderer* Renderer) const
{
	if (GetSettings()->bDebugDraw && mMovementComponent)
	{
		// Draw current tile under the character
		if (const auto& Tile = mMovementComponent->GetCurrentTile())
		{
			Renderer->SetDrawColor(255, 0, 0, 50);
			Renderer->DrawFillRectAt({ 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE },
									 Tile->GetLocalPosition());
		}

		// Draw target tile
		if (const auto& Tile = mMovementComponent->GetCurrentTile())
		{
			Renderer->SetDrawColor(0, 255, 0, 50);
			Renderer->DrawFillRectAt({ 0, 0, HALF_TILE_SIZE, HALF_TILE_SIZE },
									 Tile->GetLocalPosition());
		}

		// Draw target position
		if (mMovementComponent->IsMoving())
		{
			Renderer->SetDrawColor(255, 0, 128, 255);
			auto Offset = FVector2(HALF_TILE_SIZE, HALF_TILE_SIZE);
			Renderer->DrawPointAt(mMovementComponent->GetTargetPosition() + Offset, 4);
		}
	}

	PCharacter::Draw(Renderer);
}

void PPlayerCharacter::OnKeyDown(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case SDLK_RIGHT: // Right
		case SDLK_D:
			mInputState[0] = true;
			break;
		case SDLK_LEFT: // Left
		case SDLK_A:
			mInputState[1] = true;
			break;
		case SDLK_DOWN: // Down
		case SDLK_S:
			mInputState[2] = true;
			break;
		case SDLK_UP: // Up
		case SDLK_W:
			mInputState[3] = true;
			break;
		default:
			break;
	}

	if (mInputState.any())
	{
		bInputAllowed = false;
	}
}

void PPlayerCharacter::OnKeyUp(uint32_t KeyCode)
{
	switch (KeyCode)
	{
		case SDLK_RIGHT: // Right
		case SDLK_D:
			mInputState[0] = false;
			break;
		case SDLK_LEFT: // Left
		case SDLK_A:
			mInputState[1] = false;
			break;
		case SDLK_DOWN: // Down
		case SDLK_S:
			mInputState[2] = false;
			break;
		case SDLK_UP: // Up
		case SDLK_W:
			mInputState[3] = false;
			break;
		default:
			break;
	}

	if (mInputState.any())
	{
		bInputAllowed = true;
	}
}