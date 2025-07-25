#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Actors/Interactable.h"

class PSignPost : public PActor, public IInteractable
{
public:
    PSignPost();
    ~PSignPost() override = default;

    bool Draw(const PRenderer* Renderer) const override;
    FRect GetLocalBounds() const override;
#if !EDITOR
    void Interact(PPlayerCharacter* Player) override;
#endif
};
