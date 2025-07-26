#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Components/SpriteComponent.h"

class PSignPost : public PActor, public IInteractable
{
    PSpriteComponent* mSpriteComponent;

public:
    PSignPost();
    ~PSignPost() override = default;

    FRect GetLocalBounds() const override;
    void Interact(PPlayerCharacter* Player) override;
};
