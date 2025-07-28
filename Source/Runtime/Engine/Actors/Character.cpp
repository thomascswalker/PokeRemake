#include "Character.h"

#include "Core/Constants.h"
#include "Engine/Input.h"
#include "Engine/World.h"

#include "Collider.h"

PCharacter::PCharacter()
{
	mDrawPriority = DP_FOREGROUND;

	mMovementComponent = ConstructComponent<PCharacterMovementComponent>(this);
	if (mMovementComponent)
	{
		mMovementComponent->MovementStarted.AddRaw(this, &PCharacter::OnMovementStarted);
		mMovementComponent->MovementEnded.AddRaw(this, &PCharacter::OnMovementEnded);
		mMovementComponent->MovementDirectionChanged.AddRaw(this, &PCharacter::OnMovementEnded);
	}

	mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
	if (mSpriteComponent)
	{
		mSpriteComponent->SetYOffset(CHARACTER_OFFSET);

		auto Sprite = mSpriteComponent->GetSprite();

		Sprite->SetTexture(PTextureManager::Get(TEXTURE_GARY));
		Sprite->AddAnimation("WalkRight", {SI_WalkRight, SI_IdleRight});
		Sprite->AddAnimation("WalkLeft", {SI_WalkLeft, SI_IdleLeft});
		Sprite->AddAnimation("WalkUp", {SI_WalkUpA, SI_WalkUpB});
		Sprite->AddAnimation("WalkDown", {SI_WalkDownA, SI_WalkDownB});
		Sprite->AddAnimation("IdleRight", {SI_IdleRight});
		Sprite->AddAnimation("IdleLeft", {SI_IdleLeft});
		Sprite->AddAnimation("IdleUp", {SI_IdleUp});
		Sprite->AddAnimation("IdleDown", {SI_IdleDown});
	}
}

FRect PCharacter::GetLocalBounds() const
{
	return FRect::Block();
}

FRect PCharacter::GetWorldBounds() const
{
	return {GetWorldPosition(), FVector2::Block()};
}

void PCharacter::OnMovementStarted(EOrientation Direction)
{
	switch (Direction)
	{
	case OR_East:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("WalkRight");
			break;
		}
	case OR_West:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("WalkLeft");
			break;
		}
	case OR_North:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("WalkUp");
			break;
		}
	case OR_South:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("WalkDown");
			break;
		}
	default:
		{
			break;
		}
	}
}

void PCharacter::OnMovementEnded(EOrientation Direction)
{
	switch (Direction)
	{
	case OR_East:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("IdleRight");
			break;
		}
	case OR_West:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("IdleLeft");
			break;
		}
	case OR_North:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("IdleUp");
			break;
		}
	case OR_South:
		{
			mSpriteComponent->GetSprite()->SetCurrentAnimation("IdleDown");
			break;
		}
	default:
		{
			break;
		}
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
