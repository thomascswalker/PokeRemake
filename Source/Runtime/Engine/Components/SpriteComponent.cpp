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
        LogError("Texture is nullptr: {}", mInternalName.c_str());
        LogError("Textures:");
        for (auto Tex : PTextureManager::GetTextures() | std::views::values)
        {
            LogError("\t{}", Tex->GetName().c_str());
        }
        return false;
    }

    if (!mOwner)
    {
        LogError("Owner is invalid: {}", mInternalName.c_str());
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

JSON PSpriteComponent::Serialize() const
{
    BEGIN_SAVE_PROPERTIES(PComponent);
    SERIALIZE_MEMBER_PROPERTY(Sprite);
    SAVE_MEMBER_PROPERTY(YOffset);
    END_SAVE_PROPERTIES;
}

void PSpriteComponent::Deserialize(const JSON& Data)
{
    BEGIN_LOAD_PROPERTIES(PComponent);
    DESERIALIZE_MEMBER_PROPERTY(Sprite);
    LOAD_MEMBER_PROPERTY(YOffset, float);
}
