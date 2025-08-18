#pragma once

#include "Core/CoreFwd.h"

class ISelectable
{
public:
	virtual ~ISelectable() = default;

	virtual PSelectionComponent* GetSelectionComponent() const
	{
		return nullptr;
	}

	virtual void SetSelectionComponent(PSelectionComponent* Component) {}
};
