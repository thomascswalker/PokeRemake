#include "Character.h"

#include "Core/Constants.h"
#include "Engine/Input.h"
#include "Engine/World.h"

#include "Collider.h"

PCharacter::PCharacter()
{
	mPosition.Z = Drawing::Z_NPC;

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
		mSpriteComponent->SetOffset({0, CHARACTER_OFFSET});

		auto Sprite = mSpriteComponent->GetSprite();

		Sprite->SetTexture(TextureManager::Get(TEXTURE_GARY));
		Sprite->SetSize(16.0f);
		Sprite->SetIndexSize(16.0f);

		Sprite->AddAnimation("WalkRight", {SI_WalkRight, SI_IdleRight});
		Sprite->AddAnimation("WalkLeft", {SI_WalkLeft, SI_IdleLeft});
		Sprite->AddAnimation("WalkUp", {SI_WalkUpA, SI_WalkUpB});
		Sprite->AddAnimation("WalkDown", {SI_WalkDownA, SI_WalkDownB});
		Sprite->AddAnimation("IdleRight", {SI_IdleRight});
		Sprite->AddAnimation("IdleLeft", {SI_IdleLeft});
		Sprite->AddAnimation("IdleUp", {SI_IdleUp});
		Sprite->AddAnimation("IdleDown", {SI_IdleDown});

		Sprite->SetCurrentAnimation("IdleDown");
	}
}

FRect PCharacter::GetLocalBounds() const
{
	return FRect::Block();
}

FRect PCharacter::GetWorldBounds() const
{
	return {GetWorldPosition2D(), FVector2::Block()};
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
	for (auto Actor : GetWorld()->GetActorsAtPosition(GetPosition2D()))
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
