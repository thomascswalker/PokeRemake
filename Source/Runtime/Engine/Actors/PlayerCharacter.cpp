#include "PlayerCharacter.h"

#include "../Settings.h"
#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/ClassRegistry.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

#define PLAYER_SPEED 1.0f

void PPlayerCharacter::Start()
{
	PCharacter::Start();
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
			mMovementComponent->Move({ DOUBLE_TILE_SIZE, 0 });
		}
		else if (mInputState[1])
		{
			mMovementComponent->Move({ -DOUBLE_TILE_SIZE, 0 });
		}
		else if (mInputState[2])
		{
			mMovementComponent->Move({ 0, DOUBLE_TILE_SIZE });
		}
		else if (mInputState[3])
		{
			mMovementComponent->Move({ 0, -DOUBLE_TILE_SIZE });
		}
	}
}

void PPlayerCharacter::DebugDraw(const PRenderer* Renderer) const
{
	PCharacter::DebugDraw(Renderer);

	// Draw current tile under the character
	if (const auto& Tile = mMovementComponent->GetCurrentTile())
	{
		Renderer->SetDrawColor(255, 0, 0, 50);
		Renderer->DrawFillRectAt({ 0, 0, TILE_SIZE, TILE_SIZE },
								 Tile->GetPosition());
	}

	// Draw target tile
	if (const auto& Tile = mMovementComponent->GetCurrentTile())
	{
		Renderer->SetDrawColor(0, 255, 0, 50);
		Renderer->DrawFillRectAt({ 0, 0, TILE_SIZE, TILE_SIZE },
								 Tile->GetPosition());
	}

	// Draw target position
	if (mMovementComponent->IsMoving())
	{
		Renderer->SetDrawColor(255, 0, 128, 255);
		auto Offset = FVector2(TILE_SIZE, TILE_SIZE);
		Renderer->DrawPointAt(mMovementComponent->GetTargetPosition() + Offset, 4);
	}
}

void PPlayerCharacter::OnKeyDown(SInputEvent* Event)
{
	switch (Event->KeyDown)
	{
		case SDLK_RIGHT: // Right
		case SDLK_D:
			mInputState[0] = true;
			Event->Consume();
			break;
		case SDLK_LEFT: // Left
		case SDLK_A:
			mInputState[1] = true;
			Event->Consume();
			break;
		case SDLK_DOWN: // Down
		case SDLK_S:
			mInputState[2] = true;
			Event->Consume();
			break;
		case SDLK_UP: // Up
		case SDLK_W:
			mInputState[3] = true;
			Event->Consume();
			break;
		default:
			break;
	}

	if (mInputState.any())
	{
		bInputAllowed = false;
	}
}

void PPlayerCharacter::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_RIGHT: // Right
		case SDLK_D:
			mInputState[0] = false;
			Event->Consume();
			break;
		case SDLK_LEFT: // Left
		case SDLK_A:
			mInputState[1] = false;
			Event->Consume();
			break;
		case SDLK_DOWN: // Down
		case SDLK_S:
			mInputState[2] = false;
			Event->Consume();
			break;
		case SDLK_UP: // Up
		case SDLK_W:
			mInputState[3] = false;
			Event->Consume();
			break;
		default:
			break;
	}

	if (mInputState.any())
	{
		bInputAllowed = true;
	}
}