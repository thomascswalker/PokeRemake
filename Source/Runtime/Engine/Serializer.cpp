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
	auto World = GetWorld();
	for (const auto& ObjectData : JsonData["Objects"])
	{
		if (ObjectData.is_null())
		{
			continue; // Skip null objects
		}

		auto	ClassName = ObjectData["Class"].get<std::string>();
		PActor* NewActor = nullptr;
		if (ClassName == "PChunk")
		{
			NewActor = World->SpawnActor<PChunk>();
		}
		else if (ClassName == "PPortal")
		{
			NewActor = World->SpawnActor<PPortal>();
		}
		if (!NewActor)
		{
			LogWarning("Deserialization of actor {} not supported.", ClassName.c_str());
			continue;
		}
		NewActor->Deserialize(ObjectData);
	}
}