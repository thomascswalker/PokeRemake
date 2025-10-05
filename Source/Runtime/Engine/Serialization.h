#pragma once

#include "Core/CoreFwd.h"

class ISerializable
{
public:
	virtual ~ISerializable() = default;
	virtual JSON Serialize() const = 0;
	virtual void Deserialize(const JSON& Json) = 0;
};

namespace Serialization
{
	JSON		Serialize(const PObject* Object);
	PActor*		DeserializeActor(const JSON& Data);
	PComponent* DeserializeComponent(const JSON& Data, PActor* Owner);
}; // namespace Serialization
