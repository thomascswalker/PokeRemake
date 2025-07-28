#include "Serialization.h"

#include "Actors/Actor.h"
#include "Actors/Portal.h"
#include "Actors/SceneryActor.h"

#include "World.h"

JSON Serialization::Serialize(const PObject* Object)
{
	if (!Object->IsSerializable())
	{
		return {}; // Skip non-serializable objects
	}

	return Object->Serialize();
}

PActor* Serialization::DeserializeActor(const JSON& Data)
{
	if (Data.is_null())
	{
		return nullptr; // Skip null objects
	}

	LogDebug("Deserializing Actor: {}", Data.at("Class").get<std::string>().c_str());
	BEGIN_CONSTRUCT_ACTOR;

	CONSTRUCT_EACH_ACTOR(
		Map,
		Portal,
		SceneryActor
	);

	LogWarning("Deserialization of Actor {} not supported.", ClassName.c_str());
	return nullptr;
}

PComponent* Serialization::DeserializeComponent(const JSON& Data, PActor* Owner)
{
	if (Data.is_null())
	{
		return nullptr; // Skip null objects
	}

	LogDebug("Deserializing Component: {}", Data.at("Class").get<std::string>().c_str());
	BEGIN_CONSTRUCT_COMPONENT;

	CONSTRUCT_EACH_COMPONENT(
		SpriteComponent,
		CameraComponent,
		CharacterMovementComponent,
		CollisionComponent
	);

	LogWarning("Deserialization of Component {} not supported.", ClassName.c_str());
	return nullptr;
}
