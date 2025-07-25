#pragma once

#include "Core/CoreFwd.h"

namespace Serialization
{
	JSON	Serialize(const PObject* Object);
	PActor* Deserialize(const JSON& JsonData);
}; // namespace Serialization
