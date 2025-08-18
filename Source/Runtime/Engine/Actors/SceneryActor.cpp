#include "SceneryActor.h"

#include "Engine/World.h"
#include "Engine/Components/InteractionComponent.h"

PSceneryActor::PSceneryActor()
{
    mPosition.Z      = Drawing::Z_FG;
    mBlocking        = true;
    mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
    if (mSpriteComponent)
    {
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
    JSON Result;
    Result["Position"] = {mPosition.X, mPosition.Y};
    Result["Depth"]    = mPosition.Z;
    Result[mType]      = JSON::object();

    auto InteractData                       = mInteractionComponent->GetInteractData();
    Result[mType]["Components::1::Message"] = InteractData->Message;
    END_SAVE_PROPERTIES;
}

void PSceneryActor::Deserialize(const JSON& Data)
{
    BEGIN_LOAD_PROPERTIES(PActor);
    LOAD_MEMBER_PROPERTY(Type, std::string);
    mSpriteComponent      = GetComponent<PSpriteComponent>();
    mInteractionComponent = GetComponent<PInteractionComponent>();
}

