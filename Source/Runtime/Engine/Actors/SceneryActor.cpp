#include "SceneryActor.h"

#include "Engine/World.h"
#include "Interface/Game/GameHUD.h"

PSceneryActor::PSceneryActor()
{
    mBlocking        = true;
    mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
    if (mSpriteComponent)
    {
        // Assumes scenery actors use the standard tileset atlases.
        mSpriteComponent->SetSize(16.0f);
        mSpriteComponent->SetIndexSize(8.0f);
    }
}

PSpriteComponent* PSceneryActor::GetSpriteComponent()
{
    if (mSpriteComponent)
    {
        return mSpriteComponent;
    }
    mSpriteComponent = GetComponent<PSpriteComponent>();
    return mSpriteComponent;
}

FRect PSceneryActor::GetLocalBounds() const
{
    return FRect::Block();
}

void PSceneryActor::Interact(PPlayerCharacter* Player)
{
    GetHUD<PGameHUD>()->DialogBox("This is a signpost.");
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
}

