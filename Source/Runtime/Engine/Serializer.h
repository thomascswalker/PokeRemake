#pragma once

#include "Core/Json.h"
#include "Object.h"

class PActor;

using namespace nlohmann;

class PSerializer
{
public:
	static JSON	   Serialize(const PObject* Object);
	static PActor* Deserialize(const JSON& JsonData);
};
