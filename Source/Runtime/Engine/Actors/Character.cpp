#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

PCharacter::PCharacter()
{
	mRenderPriority = DP_FOREGROUND;

	mSprite = PSprite();
	mSprite.SetTexture(PTextureManager::Get(TEXTURE_GARY));
	mSprite.AddAnimation("WalkRight", { SI_WalkRight, SI_IdleRight });
	mSprite.AddAnimation("WalkLeft", { SI_WalkLeft, SI_IdleLeft });
	mSprite.AddAnimation("WalkUp", { SI_WalkUpA, SI_WalkUpB });
	mSprite.AddAnimation("WalkDown", { SI_WalkDownA, SI_WalkDownB });
	mSprite.AddAnimation("IdleRight", { SI_IdleRight });
	mSprite.AddAnimation("IdleLeft", { SI_IdleLeft });
	mSprite.AddAnimation("IdleUp", { SI_IdleUp });
	mSprite.AddAnimation("IdleDown", { SI_IdleDown });

	// Default to idle down animation
	mSprite.SetCurrentAnimation("IdleDown");

	mMovementComponent = ConstructComponent<PCharacterMovementComponent>(this);
	if (mMovementComponent)
	{
		mMovementComponent->MovementStarted.AddRaw(this, &PCharacter::OnMovementStarted);
		mMovementComponent->MovementEnded.AddRaw(this, &PCharacter::OnMovementEnded);
		mMovementComponent->MovementDirectionChanged.AddRaw(this, &PCharacter::OnMovementEnded);
	}
}

void PCharacter::Tick(float DeltaTime)
{
	PActor::Tick(DeltaTime);
	mSprite.Tick(DeltaTime);
}

bool PCharacter::Draw(const PRenderer* Renderer) const
{
	int32_t Index = 0;
	if (auto CurrentAnim = mSprite.GetCurrentAnimation())
	{
		Index = CurrentAnim->GetCurrentIndex();
	}

	auto Bounds = GetWorldBounds();
	Bounds.Y -= CHARACTER_OFFSET;
	Renderer->DrawSpriteAt(mSprite.GetTexture(), Bounds, Index);
	return true;
}

FRect PCharacter::GetLocalBounds() const
{
	return { 0, 0, BLOCK_SIZE, BLOCK_SIZE };
}

FRect PCharacter::GetWorldBounds() const
{
	auto WorldPosition = GetWorldPosition();
	return { WorldPosition.X, WorldPosition.Y, BLOCK_SIZE, BLOCK_SIZE };
}

void PCharacter::OnMovementStarted(EOrientation Direction)
{
	switch (Direction)
	{
		case OR_East:
			{
				mSprite.SetCurrentAnimation("WalkRight");
				break;
			}
		case OR_West:
			{
				mSprite.SetCurrentAnimation("WalkLeft");
				break;
			}
		case OR_North:
			{
				mSprite.SetCurrentAnimation("WalkUp");
				break;
			}
		case OR_South:
			{
				mSprite.SetCurrentAnimation("WalkDown");
				break;
			}
		default:
			break;
	}
}
void PCharacter::OnMovementEnded(EOrientation Direction)
{
	switch (Direction)
	{
		case OR_East:
			{
				mSprite.SetCurrentAnimation("IdleRight");
				break;
			}
		case OR_West:
			{
				mSprite.SetCurrentAnimation("IdleLeft");
				break;
			}
		case OR_North:
			{
				mSprite.SetCurrentAnimation("IdleUp");
				break;
			}
		case OR_South:
			{
				mSprite.SetCurrentAnimation("IdleDown");
				break;
			}
		default:
			break;
	}

	// Collision
	for (auto Actor : GetWorld()->GetActorsAtPosition(mPosition))
	{
		if (Actor == this)
		{
			continue;
		}
		for (auto Comp : Actor->GetComponents())
		{
			if (auto Coll = dynamic_cast<ICollider*>(Comp))
			{
				Coll->ProcessCollision();
			}
		}
	}
}
