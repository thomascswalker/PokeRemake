#pragma once

#include "Logging.h"
#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

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
		return;                             \
	}

#define SAVE_MEMBER_PROPERTY(Json, Property) \
	Json[#Property] = m##Property

#define LOAD_MEMBER_PROPERTY(Json, Property, Type) \
	if (Json.contains(#Property))                  \
	{                                              \
		m##Property = Json[#Property].get<Type>(); \
	}                                              \
	else                                           \
	{                                              \
		OnPropertyMissing(Json, #Property);        \
	}
//