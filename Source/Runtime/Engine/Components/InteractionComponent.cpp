#include "InteractionComponent.h"

#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/World.h"

void PInteractionComponent::Interact(PPlayerCharacter* Player)
{
	SGameEvent Event(this, EGameEventType::Dialog, &mData);
	// GEngine->GetGame()->HandleGameEvent(Event);
	GGameMode->HandleGameEvent(Event);
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
