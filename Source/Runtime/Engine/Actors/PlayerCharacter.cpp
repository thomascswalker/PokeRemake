#include "PlayerCharacter.h"

#include "Core/Constants.h"
#include "Engine/Components/InteractionComponent.h"
#include "Engine/Input.h"
#include "Engine/MapManager.h"
#include "Engine/World.h"

#include "Interactable.h"

#define PLAYER_SPEED 1.0f

PPlayerCharacter::PPlayerCharacter()
{
	mPosition.Z = Drawing::Z_Player;
	mCameraComponent = GWorld->ConstructComponent<PCameraComponent>(this);
}

void PPlayerCharacter::Tick(float DeltaTime)
{
	if (!CanMove())
	{
		return;
	}

	if (!mMovementComponent->IsMoving() && mInputState.any())
	{
		if (mInputState[DPAD_LEFT])
		{
			mMovementComponent->Move({ -BLOCK_SIZE, 0 });
		}
		else if (mInputState[DPAD_RIGHT])
		{
			mMovementComponent->Move({ BLOCK_SIZE, 0 });
		}
		else if (mInputState[DPAD_UP])
		{
			mMovementComponent->Move({ 0, -BLOCK_SIZE });
		}
		else if (mInputState[DPAD_DOWN])
		{
			mMovementComponent->Move({ 0, BLOCK_SIZE });
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
	Renderer->DrawFillRectAt({ TargetPosition.X, TargetPosition.Y, BLOCK_SIZE, BLOCK_SIZE });

	// Draw target position
	if (mMovementComponent->IsMoving())
	{
		Renderer->SetDrawColor(255, 0, 128, 255);
		auto Offset = FVector2(BLOCK_SIZE, BLOCK_SIZE);
		Renderer->DrawPointAt(TargetPosition + Offset, 4);
	}

	return true;
}

bool PPlayerCharacter::CanMove() const
{
	return mInputAllowed;
}

void PPlayerCharacter::SetCanMove(bool State)
{
	mInputAllowed = State;
}

bool PPlayerCharacter::TryMove(EDPad Direction)
{
	switch (Direction)
	{
		case DPAD_UP:
			{
				mInputState[DPAD_UP] = true;
				break;
			}
		case DPAD_LEFT:
			{
				mInputState[DPAD_LEFT] = true;
				break;
			}
		case DPAD_DOWN:
			{
				mInputState[DPAD_DOWN] = true;
				break;
			}
		case DPAD_RIGHT:
			{
				mInputState[DPAD_RIGHT] = true;
				break;
			}
		default:
			return false;
	}

	mInputAllowed = mInputState.any();
	return mInputAllowed;
}

bool PPlayerCharacter::TryStop(EDPad Direction)
{
	switch (Direction)
	{
		case DPAD_UP:
			{
				mInputState[DPAD_UP] = false;
				break;
			}
		case DPAD_LEFT:
			{
				mInputState[DPAD_LEFT] = false;
				break;
			}
		case DPAD_DOWN:
			{
				mInputState[DPAD_DOWN] = false;
				break;
			}
		case DPAD_RIGHT:
			{
				mInputState[DPAD_RIGHT] = false;
				break;
			}
		default:
			return false;
	}

	mInputAllowed = !mInputState.any();
	return mInputAllowed;
}

void PPlayerCharacter::Interact()
{
	FVector2 Forward = DirectionToVector(GetMovementComponent()->GetMovementDirection());
	FVector2 TargetPosition = GetPosition2D() + (Forward * BLOCK_SIZE);

	if (auto Actor = GWorld->GetActorAtPosition(TargetPosition))
	{
		if (auto Interactable = dynamic_cast<IInteractable*>(Actor))
		{
			Interactable->HandleInteraction();
		}
	}
}
