#include "Serializer.h"

#include "Actors/Actor.h"
#include "Actors/Portal.h"
#include "World.h"

JSON PSerializer::Serialize(const PObject* Object)
{
	if (!Object->IsSerializable())
	{
		return {}; // Skip non-serializable objects
	}

	return Object->Serialize();
}

PActor* PSerializer::Deserialize(const JSON& JsonData)
{
	if (JsonData.is_null())
	{
		return nullptr; // Skip null objects
	}

	auto ClassName = JsonData.at("Class").get<std::string>();
	if (ClassName == "PMap")
	{
		return SpawnActor<PMap>(JsonData);
	}
	if (ClassName == "PPortal")
	{
		return SpawnActor<PPortal>(JsonData);
	}

	LogWarning("Deserialization of actor {} not supported.", ClassName.c_str());
	return nullptr;
}