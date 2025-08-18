#include "PlayerCharacter.h"

#include "Interactable.h"

#include "Core/Constants.h"
#include "Engine/Input.h"
#include "Engine/World.h"
#include "Engine/Components/InteractionComponent.h"

#define PLAYER_SPEED 1.0f

PPlayerCharacter::PPlayerCharacter()
{
	mCameraComponent = GetWorld()->ConstructComponent<PCameraComponent>(this);

	mSpriteComponent->GetSprite()->SetTexture(PTextureManager::Get(TEXTURE_ASH));
	if (mSpriteComponent)
	{
		mSpriteComponent->SetDrawPriority(Z_Player);
	}
}

void PPlayerCharacter::Tick(float DeltaTime)
{
	if (!mMovementComponent->IsMoving() && mInputState.any())
	{
		if (mInputState[0])
		{
			mMovementComponent->Move({BLOCK_SIZE, 0});
		}
		else if (mInputState[1])
		{
			mMovementComponent->Move({-BLOCK_SIZE, 0});
		}
		else if (mInputState[2])
		{
			mMovementComponent->Move({0, BLOCK_SIZE});
		}
		else if (mInputState[3])
		{
			mMovementComponent->Move({0, -BLOCK_SIZE});
		}
	}
}

bool PPlayerCharacter::DebugDraw(const PRenderer* Renderer) const
{
	// Draw current block under the character
	Renderer->SetDrawColor(255, 0, 0, 50);
	Renderer->DrawFillRectAt(GetWorldBounds());

	auto TargetPosition = mMovementComponent->GetTargetPosition();

	// Draw target block
	Renderer->SetDrawColor(0, 255, 0, 50);
	Renderer->DrawFillRectAt({TargetPosition.X, TargetPosition.Y, BLOCK_SIZE, BLOCK_SIZE});

	// Draw target position
	if (mMovementComponent->IsMoving())
	{
		Renderer->SetDrawColor(255, 0, 128, 255);
		auto Offset = FVector2(BLOCK_SIZE, BLOCK_SIZE);
		Renderer->DrawPointAt(TargetPosition + Offset, 4);
	}

	return true;
}

void PPlayerCharacter::OnKeyDown(SInputEvent* Event)
{
	switch (Event->KeyDown)
	{
	case SDLK_RIGHT: // Right
	case SDLK_D:
		{
			mInputState[0] = true;
			Event->Consume();
			break;
		}
	case SDLK_LEFT: // Left
	case SDLK_A:
		{
			mInputState[1] = true;
			Event->Consume();
			break;
		}
	case SDLK_DOWN: // Down
	case SDLK_S:
		{
			mInputState[2] = true;
			Event->Consume();
			break;
		}
	case SDLK_UP: // Up
	case SDLK_W:
		{
			mInputState[3] = true;
			Event->Consume();
			break;
		}
	default: break;
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
		{
			mInputState[0] = false;
			Event->Consume();
			break;
		}
	case SDLK_LEFT: // Left
	case SDLK_A:
		{
			mInputState[1] = false;
			Event->Consume();
			break;
		}
	case SDLK_DOWN: // Down
	case SDLK_S:
		{
			mInputState[2] = false;
			Event->Consume();
			break;
		}
	case SDLK_UP: // Up
	case SDLK_W:
		{
			mInputState[3] = false;
			Event->Consume();
			break;
		}
	case SDLK_E:
		{
			Interact();
			Event->Consume();
			break;
		}
	default: break;
	}

	if (mInputState.any())
	{
		bInputAllowed = true;
	}
}

void PPlayerCharacter::Interact()
{
	FVector2 Forward        = DirectionToVector(GetMovementComponent()->GetMovementDirection());
	FVector2 TargetPosition = mPosition + (Forward * BLOCK_SIZE);

	if (auto Actor = GetWorld()->GetActorAtPosition(TargetPosition))
	{
		if (auto Interactable = dynamic_cast<IInteractable*>(Actor))
		{
			if (auto Component = Interactable->GetInteractionComponent())
			{
				Component->Interact(this);
			}
		}
	}
}
