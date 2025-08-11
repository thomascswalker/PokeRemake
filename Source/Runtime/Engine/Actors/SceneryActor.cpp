#include "SceneryActor.h"

#include "Engine/World.h"

PSceneryActor::PSceneryActor()
{
    mBlocking        = true;
    mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
    if (mSpriteComponent)
    {
        mSpriteComponent->SetDrawPriority(Z_FG);

        // Assumes scenery actors use the standard tileset atlases.
        mSpriteComponent->SetSize(16.0f);
        mSpriteComponent->SetIndexSize(8.0f);
    }

    mInteractionComponent = ConstructComponent<PInteractionComponent>(this);
}

PSpriteComponent* PSceneryActor::GetSpriteComponent()
{
    return mSpriteComponent;
}

PInteractionComponent* PSceneryActor::GetInteractionComponent()
{
    return mInteractionComponent;
}

FRect PSceneryActor::GetLocalBounds() const
{
    return FRect::Block();
}

JSON PSceneryActor::Serialize() const
{
    BEGIN_SAVE_PROPERTIES(PActor);
    SAVE_MEMBER_PROPERTY(Type);
    END_SAVE_PROPERTIES;
}

void PSceneryActor::Deserialize(const JSON& Data)
{
    BEGIN_LOAD_PROPERTIES(PActor);
    LOAD_MEMBER_PROPERTY(Type, ESceneryType);
    mSpriteComponent      = GetComponent<PSpriteComponent>();
    mInteractionComponent = GetComponent<PInteractionComponent>();
}

