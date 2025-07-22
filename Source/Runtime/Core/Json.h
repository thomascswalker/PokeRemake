#pragma once

#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

#define CHECK_PROPERTY(Json, Property)                  \
	if (!Json.contains(#Property))                      \
	{                                                   \
		LogError("Property {} is missing.", #Property); \
		return;                                         \
	}

#define SAVE_PROPERTY(Json, Property) \
	Json[#Property] = m##Property

#define LOAD_PROPERTY(Json, Property, Type)               \
	if (Json.contains(#Property))                         \
	{                                                     \
		m##Property = Json[#Property].get<Type>();        \
	}                                                     \
	else                                                  \
	{                                                     \
		LogWarning("Property {} is missing.", #Property); \
	}
//