#pragma once
#include "Component.h"

class PSpriteComponent : public PComponent, public IDrawable
{
    PSprite mSprite;
    float mYOffset = 0;

public:
    PSpriteComponent();
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
};
