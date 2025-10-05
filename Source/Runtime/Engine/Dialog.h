#pragma once
#include "Serialization.h"

struct SDialogContext : ISerializable
{
	std::string Message;

	JSON Serialize() const override
	{
		JSON Result;
		Result["Message"] = Message;
		return Result;
	}

	void Deserialize(const JSON& Json) override
	{
		Message = Json["Message"];
	}
};
