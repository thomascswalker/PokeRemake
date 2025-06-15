#include "Serializer.h"

#include "Core/Logging.h"
#include "World.h"

void PSerializer::Serialize(const PObject* Object)
{
	if (!Object->IsSerializable())
	{
		return; // Skip non-serializable objects
	}
	mSerializedData["Objects"].push_back(Object->Serialize());
}

void PSerializer::Deserialize(const json& JsonData)
{
	auto World = GetWorld();
	for (const auto& ObjectData : JsonData["Objects"])
	{
		if (ObjectData.is_null())
		{
			continue; // Skip null objects
		}
		if (ObjectData["Class"] == "6PChunk")
		{
			LogDebug("Loading chunk: {}", ObjectData["Name"].get<std::string>().c_str());
		}
	}
}