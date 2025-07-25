#pragma once

class PPlayerCharacter;

class IInteractable
{
public:
	virtual ~IInteractable();
	virtual void Interact() = 0;
	virtual bool CanInteract(PPlayerCharacter* Player) = 0;
};