#include "InteractionComponent.h"

#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/World.h"

void PInteractionComponent::Interact(PPlayerCharacter* Player)
{
	if (auto Interactable = dynamic_cast<IInteractable*>(mOwner))
	{
		Interactable->HandleInteraction();
	}
	else
	{
		LogWarning("Owner of PInteractionComponent '{}' does not subclass IInteractable.", mOwner->GetInternalName().c_str());
	}
}
