#pragma once

#include <string>

#include "Json.h"

using namespace nlohmann;

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

template <typename T>
struct StripType
{
	typedef T Type;
};

template <typename T>
struct StripType<const T&>
{
	typedef T Type;
};

template <typename T>
struct StripType<const T* const>
{
	typedef T Type;
};

class PObject;

struct SClass
{
	std::string Name;
	size_t TypeId;
	std::function<PObject*()> Constructor;
	std::function<PObject*(const JSON&)> JsonConstructor;
};

#define IS_SUBCLASS_OF(Class) class T, class = std::enable_if_t<std::is_base_of_v<Class, T>>
