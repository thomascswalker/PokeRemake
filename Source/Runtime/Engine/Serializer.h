#pragma once

#include "Object.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;

class PSerializer
{
	json mSerializedData;

public:
	PSerializer() { mSerializedData["Objects"] = json::array(); }
	void Serialize(const PObject* Object);
	json GetSerializedData() const { return mSerializedData; }
	void Clear() { mSerializedData.clear(); }
	void Deserialize(const json& JsonData);
};
