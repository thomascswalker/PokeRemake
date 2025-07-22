#include "Serializer.h"

#include "Actors/Portal.h"
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
	for (const auto& ObjectData : JsonData["Objects"])
	{
		if (ObjectData.is_null())
		{
			continue; // Skip null objects
		}

		auto ClassName = ObjectData["Class"].get<std::string>();
		if (ClassName == "PChunk")
		{
			SpawnActor<PChunk>(ObjectData);
		}
		else if (ClassName == "PPortal")
		{
			SpawnActor<PPortal>(ObjectData);
		}
		else
		{
			LogWarning("Deserialization of actor {} not supported.", ClassName.c_str());
		}
	}
}