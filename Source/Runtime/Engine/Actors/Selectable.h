#pragma once

#include "Core/CoreFwd.h"

class ISelectable
{
protected:
	bool mSelectable = true;

public:
	virtual ~ISelectable() = default;

	virtual PSelectionComponent* GetSelectionComponent() const
	{
		return nullptr;
	}

	virtual void SetSelectionComponent(PSelectionComponent* Component) {}

	bool GetSelectable() const
	{
		return mSelectable;
	}
};
