#include "InteractionComponent.h"

#include "Engine/World.h"
#include "Interface/Game/GameHUD.h"

void PInteractionComponent::Interact(PPlayerCharacter* Player)
{
    LogDebug("Interact: {}, {}", mData.Name.c_str(), mData.Message.c_str());
    LogDebug("Initialized: {}", mInitialized ? "true" : "false");
    GetHUD<PGameHUD>()->DialogBox(mData.Message);
}

void PInteractionComponent::Deserialize(const JSON& Data)
{
    PComponent::Deserialize(Data);
    mData.Name    = Data["Name"];
    mData.Message = Data["Message"];
    LogDebug("Interact: {}, {}", mData.Name.c_str(), mData.Message.c_str());

    mInitialized = true;
}
