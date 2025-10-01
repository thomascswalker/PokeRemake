#pragma once

#include "Core/CoreFwd.h"

struct SInteractContext
{
	std::string Name = "Default Name";
	std::string Message = "Default message.";

	SInteractContext() = default;
	SInteractContext(const std::string& InName, const std::string& InMessage) : Name(InName), Message(InMessage) {}

	SInteractContext(const SInteractContext& Other)
	{
		Name = Other.Name;
	};

	SInteractContext& operator=(const SInteractContext& Other)
	{
		Name = Other.Name;
		return *this;
	}
};

class IInteractable
{
public:
	virtual ~IInteractable() {}
	virtual PInteractionComponent* GetInteractionComponent() = 0;
};
