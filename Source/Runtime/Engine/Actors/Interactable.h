#pragma once

#include "Core/CoreFwd.h"

struct SInteractData
{
	std::string Name = "Default Name";
	std::string Message = "Default message.";

	SInteractData() = default;
	SInteractData(const std::string& InName, const std::string& InMessage) : Name(InName), Message(InMessage) {}

	SInteractData(const SInteractData& Other)
	{
		Name = Other.Name;
	};

	SInteractData& operator=(const SInteractData& Other)
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
