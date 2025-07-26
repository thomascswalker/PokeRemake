#include "SpriteComponent.h"

PSpriteComponent::PSpriteComponent()
{
    mDrawPriority = DP_FOREGROUND;
}

bool PSpriteComponent::Draw(const PRenderer* Renderer) const
{
    auto Texture = mSprite.GetTexture();
    if (!Texture)
    {
        return false;
    }

    auto Bounds = mOwner->GetWorldBounds();
    Bounds.Y -= mYOffset;

    const FRect Source = mSprite.GetCurrentRect();
    Renderer->DrawTextureAt(Texture, Source, Bounds);

    return true;
}

void PSpriteComponent::Tick(float DeltaTime)
{
    mSprite.Tick(DeltaTime);
}
