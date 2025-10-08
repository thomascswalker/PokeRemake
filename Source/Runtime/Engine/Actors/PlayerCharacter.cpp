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
		if (mInputState[0])
		{
			mMovementComponent->Move({ BLOCK_SIZE, 0 });
		}
		else if (mInputState[1])
		{
			mMovementComponent->Move({ -BLOCK_SIZE, 0 });
		}
		else if (mInputState[2])
		{
			mMovementComponent->Move({ 0, BLOCK_SIZE });
		}
		else if (mInputState[3])
		{
			mMovementComponent->Move({ 0, -BLOCK_SIZE });
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
	return GMapManager->GetState() == MS_Loaded;
}

bool PPlayerCharacter::TryMove(EDPad Direction)
{
	switch (Direction)
	{
		case DPAD_RIGHT:
			{
				mInputState[0] = true;
				break;
			}
		case DPAD_LEFT:
			{
				mInputState[1] = true;
				break;
			}
		case DPAD_DOWN:
			{
				mInputState[2] = true;
				break;
			}
		case DPAD_UP:
			{
				mInputState[3] = true;
				break;
			}
		default:
			break;
	}

	mInputAllowed = !mInputState.any();
	return mInputAllowed;
}

bool PPlayerCharacter::TryStop(EDPad Direction)
{
	switch (Direction)
	{
		case DPAD_RIGHT:
			{
				mInputState[0] = false;
				break;
			}
		case DPAD_LEFT:
			{
				mInputState[1] = false;
				break;
			}
		case DPAD_DOWN:
			{
				mInputState[2] = false;
				break;
			}
		case DPAD_UP:
			{
				mInputState[3] = false;
				break;
			}
		default:
			break;
	}

	mInputAllowed = mInputState.any();
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
