#pragma once

#include "Core/CoreFwd.h"

struct SInteractData
{
	std::string Name;
	std::string Message;
};

class IInteractable
{
public:
	virtual ~IInteractable() {}
	virtual void Interact(PPlayerCharacter* Player) {};
};
