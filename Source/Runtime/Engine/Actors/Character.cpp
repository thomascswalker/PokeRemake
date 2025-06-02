#include "Character.h"

#include "Engine/World.h"

PCharacter::PCharacter()
{
	const float Size = 64.0f;
	mBounds = FRect(0, 0, Size, Size);
}

void PCharacter::Start() {}

void PCharacter::Tick(float DeltaTime)
{
	PActor::Tick(DeltaTime);
}

void PCharacter::Draw(const PRenderer* Renderer) const
{
	int Index = 0;
	if (mVelocity.X > 0)
	{
		Index = 9; // Walk right
	}
	else if (mVelocity.X < 0)
	{
		Index = 7; // Walk left
	}
	else if (mVelocity.Y > 0)
	{
		Index = 0; // Walk down
	}
	else if (mVelocity.Y < 0)
	{
		Index = 3; // Walk up
	}
	Renderer->DrawSpriteAt(PTextureManager::Get(TEXTURE_ASH), mBounds, { mPosition.X, mPosition.Y },
						   Index);
}
