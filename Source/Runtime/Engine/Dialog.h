#pragma once

#include "Serialization.h"

DECLARE_MULTICAST_DELEGATE(DDialogCompleted);

struct SDialogContext : ISerializable
{
	std::string		 Message;
	DDialogCompleted DialogCompleted;

	SDialogContext() = default;
	SDialogContext(const std::string& InMessage) : Message(InMessage) {}

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
