#pragma once
#include "Actors/Trainer.h"
#include "Engine/Serialization.h"

class PGameSerializer : public PSerializer
{
	PActor*		DeserializeActor(const JSON& Data) override;
	PComponent* DeserializeComponent(const JSON& Data, PActor* Owner) override;
};

inline PActor* PGameSerializer::DeserializeActor(const JSON& Data)
{
	if (PActor* Actor = PSerializer::DeserializeActor(Data))
	{
		return Actor;
	}

	CHECK_PROPERTY(Data, Class);

	BEGIN_CONSTRUCT_ACTOR;

	CONSTRUCT_EACH_ACTOR(
		Trainer);

	return nullptr;
}

inline PComponent* PGameSerializer::DeserializeComponent(const JSON& Data, PActor* Owner)
{
	if (PComponent* Component = PSerializer::DeserializeComponent(Data, Owner))
	{
		return Component;
	}
	return nullptr;
}