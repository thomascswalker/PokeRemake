#include "InteractionComponent.h"

#include "../../../Game/Interface/GameHUD.h"
#include "Engine/World.h"

void PInteractionComponent::Interact(PPlayerCharacter* Player)
{
	// GetHUD()->DialogBox(mData.Message);
	LogWarning("Reimplement dialog box!");
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
