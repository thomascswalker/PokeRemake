// https://gamedev.stackexchange.com/questions/172286/how-to-parse-user-defined-types-from-string

#pragma once

#include "Core/Logging.h"

#include <map>
#include <string>

class PObject;

class PClassRegistry
{
	using JsonConstructor = PObject* (*)(const json&);
	std::map<std::string, JsonConstructor> mRegistry;

protected:
	PClassRegistry() { mRegistry.clear(); }

public:
	static PClassRegistry& Instance()
	{
		static PClassRegistry Inst;
		return Inst;
	}

	bool RegisterClass(const char* type_id, JsonConstructor ConstructorFunction)
	{
		if (mRegistry[type_id])
		{
			return true; // Already registered
		}
		mRegistry[type_id] = ConstructorFunction;
		if (!mRegistry[type_id])
		{
			LogError("Failed to register class '%s' in class registry.", type_id);
			return false;
		}
		return true;
	}

	PObject* Construct(const char* type_id, const json& JsonData)
	{
		if (!mRegistry[type_id])
		{
			LogError("Constructor for type '%s' not found in class registry.", type_id);
			return nullptr;
		}
		return mRegistry[type_id](JsonData);
	}
};

#define REGISTER_CLASS(Class)                                                  \
	static bool bRegistered##Class = PClassRegistry::Instance().RegisterClass( \
		typeid(Class).name(),                                                  \
		[](const json& JsonData) -> PObject* { return new Class(JsonData); });
