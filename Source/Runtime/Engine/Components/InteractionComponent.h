#pragma once
#include "Component.h"

#include "Engine/Actors/Interactable.h"

class PInteractionComponent : public PComponent
{
    SInteractContext mData;
    bool mInitialized = false;

public:
    PInteractionComponent()           = default;
    ~PInteractionComponent() override = default;

    PInteractionComponent(const PInteractionComponent& Other)
    {
        mData = Other.mData;
    }

    PInteractionComponent& operator=(const PInteractionComponent& Other)
    {
        mData = Other.mData;
        return *this;
    }

    SInteractContext* GetInteractData()
    {
        return &mData;
    }

    void Interact(PPlayerCharacter* Player);
    void Deserialize(const JSON& Data) override;

#if _EDITOR
	void InitializeParameters() override;
#endif
};
