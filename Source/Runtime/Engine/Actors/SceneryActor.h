#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Components/SpriteComponent.h"

namespace SceneryTypes
{
#define DECLARE_TYPE(X) inline const char* X = PREPEND($, X)
    DECLARE_TYPE(SignPost);
    DECLARE_TYPE(Tree);
#undef DECLARE_TYPE
}

class PSceneryActor : public PActor, public IInteractable
{
    std::string mType;
    PSpriteComponent* mSpriteComponent;
    PInteractionComponent* mInteractionComponent;

public:
    PSceneryActor();
    ~PSceneryActor() override = default;

    PSpriteComponent* GetSpriteComponent();
    PInteractionComponent* GetInteractionComponent() override;

    FRect GetLocalBounds() const override;

    JSON Serialize() const override;
    void Deserialize(const JSON& Data) override;
};
