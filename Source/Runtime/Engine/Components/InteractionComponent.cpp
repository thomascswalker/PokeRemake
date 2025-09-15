#include "InteractionComponent.h"

#include "Engine/World.h"
#include "Interface/Game/GameHUD.h"

void PInteractionComponent::Interact(PPlayerCharacter* Player)
{
	GetHUD<PGameHUD>()->DialogBox(mData.Message);
}

void PInteractionComponent::Deserialize(const JSON& Data)
{
	PComponent::Deserialize(Data);
	mData.Name = Data["Name"];
	mData.Message = Data["Message"];
	mInitialized = true;
}

#if _EDITOR

void PInteractionComponent::InitializeParameters()
{
	PComponent::InitializeParameters();

	AddParameter("Name", &mData.Name, PT_String);
	AddParameter("Message", &mData.Message, PT_String);
}

#endif
