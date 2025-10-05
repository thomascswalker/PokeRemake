#pragma once

#include "Core/CoreFwd.h"

class IInteractable
{
public:
	virtual ~IInteractable() {}
	virtual void HandleInteraction() {}
};
