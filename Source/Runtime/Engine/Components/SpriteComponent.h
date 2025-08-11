#pragma once
#include "Component.h"

class PSpriteComponent : public PComponent, public IDrawable
{
    PSprite mSprite;
    float mYOffset = 0;

public:
    PSpriteComponent();

    PSpriteComponent(const PSpriteComponent& other)
        : PComponent{other},
          mSprite{other.mSprite},
          mYOffset{other.mYOffset} {}

    PSpriteComponent(PSpriteComponent&& other) noexcept
        : PComponent{std::move(other)},
          mSprite{std::move(other.mSprite)},
          mYOffset{other.mYOffset} {}

    PSpriteComponent& operator=(const PSpriteComponent& other)
    {
        if (this == &other)
            return *this;
        PComponent::operator =(other);
        mSprite  = other.mSprite;
        mYOffset = other.mYOffset;
        return *this;
    }

    PSpriteComponent& operator=(PSpriteComponent&& other) noexcept
    {
        if (this == &other)
            return *this;
        PComponent::operator =(std::move(other));
        mSprite  = std::move(other.mSprite);
        mYOffset = other.mYOffset;
        return *this;
    }

    ~PSpriteComponent() override = default;

    bool Draw(const PRenderer* Renderer) const override;
    void Tick(float DeltaTime) override;

    void SetYOffset(float Offset)
    {
        mYOffset = Offset;
    }

    PSprite* GetSprite()
    {
        return &mSprite;
    }

    void SetSize(float Size)
    {
        mSprite.SetSize(Size);
    }

    void SetIndexSize(float Size)
    {
        mSprite.SetIndexSize(Size);
    }

    JSON Serialize() const override;
    void Deserialize(const JSON& Data) override;
};
