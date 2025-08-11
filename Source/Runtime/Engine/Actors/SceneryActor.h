#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Components/SpriteComponent.h"
#include "Engine/Components/InteractionComponent.h"

enum ESceneryType
{
    ST_Default  = 0,
    ST_SignPost = 1
};

class PSceneryActor : public PActor, public IInteractable
{
    ESceneryType mType;
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
