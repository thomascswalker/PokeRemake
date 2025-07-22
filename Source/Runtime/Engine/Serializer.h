#pragma once

#include "Object.h"
#include "nlohmann/json.hpp"

class PActor;

using namespace nlohmann;

class PSerializer
{
public:
	static json	   Serialize(const PObject* Object);
	static PActor* Deserialize(const json& JsonData);
};
