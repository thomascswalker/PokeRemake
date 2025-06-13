#pragma once

#include "Actors/Chunk.h"
#include "Object.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;

class PSerializer
{
	json mSerializedData;

public:
	PSerializer() { mSerializedData["Objects"] = json::array(); }
	void Serialize(const PObject* Object)
	{
		if (!Object->IsSerializable())
		{
			return; // Skip non-serializable objects
		}
		mSerializedData["Objects"].push_back(Object->Serialize());
	}
	json GetSerializedData() const { return mSerializedData; }
};
