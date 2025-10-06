#pragma once

#include "Core/CoreFwd.h"

class ISerializable
{
public:
	virtual ~ISerializable() = default;
	virtual JSON Serialize() const = 0;
	virtual void Deserialize(const JSON& Json) = 0;
};

class PSerializer
{
public:
	PSerializer() = default;
	virtual ~PSerializer() = default;

	virtual PActor*		DeserializeActor(const JSON& Data);
	virtual PComponent* DeserializeComponent(const JSON& Data, PActor* Owner);
};

extern PSerializer* GSerializer;