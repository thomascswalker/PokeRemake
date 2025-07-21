#pragma once

#include "nlohmann/json.hpp"

using JSON = nlohmann::json;

#define CHECK_PROPERTY(Json, Property)                   \
	if (!Json.contains(Property))                        \
	{                                                    \
		LogWarning("Property {} is missing.", Property); \
		return;                                          \
	}

//