#pragma once

#include "Actor.h"
#include "Interactable.h"

class PSignPost : public PActor, public IInteractable
{
public:
    PSignPost();
    ~PSignPost() override = default;

    bool Draw(const PRenderer* Renderer) const override;
    FRect GetLocalBounds() const override;
    void Interact(PPlayerCharacter* Player) override;
};
