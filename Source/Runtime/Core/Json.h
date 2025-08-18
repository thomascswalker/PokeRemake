#pragma once

#include "Logging.h"
#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

void ExpandRef(JSON* Json, const std::string& Ref, const JSON& Override);

void Expand(JSON* Json);

inline void OnPropertyMissing(const JSON& Json, const char* Property)
{
	std::vector<std::string> Keys;
	for (auto Item : Json.items())
	{
		Keys.push_back(Item.key());
	}
	std::string KeysString;
	for (auto Item : Keys)
	{
		KeysString += Item + ", ";
	}
	LogWarning("Property {} is missing: [{}]", Property, KeysString.c_str());
}

#define CHECK_PROPERTY(Json, Property)      \
	if (!Json.contains(#Property))          \
	{                                       \
		OnPropertyMissing(Json, #Property); \
	}

#define BEGIN_SAVE_PROPERTIES(Parent) JSON Result = Parent::Serialize()
#define END_SAVE_PROPERTIES return Result

#define BEGIN_LOAD_PROPERTIES(Parent) Parent::Deserialize(Data)

#define SAVE_MEMBER_PROPERTY(Property) \
	Result[#Property] = m##Property

#define LOAD_MEMBER_PROPERTY(Property, Type) \
	if (Data.contains(#Property))                  \
	{                                              \
		m##Property = Data[#Property].get<Type>(); \
	}                                              \
	else                                           \
	{                                              \
		OnPropertyMissing(Data, #Property);        \
	}

#define SERIALIZE_MEMBER_PROPERTY(Property) \
	Result[#Property] = m##Property.Serialize()

#define DESERIALIZE_MEMBER_PROPERTY(Property) \
	m##Property.Deserialize(Data[#Property])
