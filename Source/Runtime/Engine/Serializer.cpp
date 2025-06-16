#include "Serializer.h"

#include "ClassRegistry.h"
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

		auto ClassName = ObjectData["Class"].get<std::string>();
		if (ClassName.find("PChunk") != std::string::npos)
		{
			World->SpawnActor<PChunk>(ObjectData);
		}
	}
}