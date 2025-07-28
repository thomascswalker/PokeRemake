#pragma once

#include "Core/CoreFwd.h"

namespace Serialization
{
	JSON Serialize(const PObject* Object);
	PActor* DeserializeActor(const JSON& Data);
	PComponent* DeserializeComponent(const JSON& Data, PActor* Owner);
}; // namespace Serialization
