#pragma once

#include "nlohmann/json.hpp"

#include "Array.h"

#define ANCHOR "&"	// Reference to ActorDef
#define HASH   "#"	// Reference to TextDef
#define ARROW  "->" // Pointer to child actor

using JSON = nlohmann::json;

void ExpandArrow(JSON* Out, const std::string& Key, const JSON& Value);

void ExpandAnchor(JSON* Out, const std::string& Ref, const JSON& Override);

void Expand(JSON* Out);

void OnPropertyMissing(const JSON& Json, const char* Property);

#define CHECK_PROPERTY(Json, Property)      \
	if (!Json.contains(#Property))          \
	{                                       \
		OnPropertyMissing(Json, #Property); \
	}

#define BEGIN_SAVE_PROPERTIES(Parent) JSON Result = Parent::Serialize()
#define END_SAVE_PROPERTIES			  return Result

#define BEGIN_LOAD_PROPERTIES(Parent) Parent::Deserialize(Data)

#define SAVE_MEMBER_PROPERTY(Property) \
	Result[#Property] = m##Property

#define LOAD_MEMBER_PROPERTY(Property, Type)       \
	if (Data.contains(#Property))                  \
	{                                              \
		m##Property = Data[#Property].get<Type>(); \
	}                                              \
	else                                           \
	{                                              \
		OnPropertyMissing(Data, #Property);        \
	}

#define SERIALIZE_MEMBER_PROPERTY(Property)     \
	Result[#Property] = m##Property.Serialize()

#define DESERIALIZE_MEMBER_PROPERTY(Property) \
	m##Property.Deserialize(Data[#Property])
