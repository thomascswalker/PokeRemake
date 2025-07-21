#include "Character.h"

#include "Core/Constants.h"
#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Engine/World.h"

void PCharacter::Start()
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

	mMovementComponent = GetWorld()->ConstructComponent<PCharacterMovementComponent>(this);
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

void PCharacter::OnMovementStarted(EMovementDirection Direction)
{
	switch (Direction)
	{
		case MD_Right:
			{
				mSprite.SetCurrentAnimation("WalkRight");
				break;
			}
		case MD_Left:
			{
				mSprite.SetCurrentAnimation("WalkLeft");
				break;
			}
		case MD_Up:
			{
				mSprite.SetCurrentAnimation("WalkUp");
				break;
			}
		case MD_Down:
			{
				mSprite.SetCurrentAnimation("WalkDown");
				break;
			}
		default:
			break;
	}
}
void PCharacter::OnMovementEnded(EMovementDirection Direction)
{
	switch (Direction)
	{
		case MD_Right:
			{
				mSprite.SetCurrentAnimation("IdleRight");
				break;
			}
		case MD_Left:
			{
				mSprite.SetCurrentAnimation("IdleLeft");
				break;
			}
		case MD_Up:
			{
				mSprite.SetCurrentAnimation("IdleUp");
				break;
			}
		case MD_Down:
			{
				mSprite.SetCurrentAnimation("IdleDown");
				break;
			}
		default:
			break;
	}
}
