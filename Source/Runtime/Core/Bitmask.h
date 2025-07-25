#pragma once

#include <cstdint>

#define DEFINE_BITMASK_OPERATORS(T)                                           \
	inline constexpr T operator|(T a, T b)                                    \
	{                                                                         \
		return static_cast<T>(static_cast<int>(a) | static_cast<int>(b));     \
	}                                                                         \
	inline constexpr T operator&(T a, T b)                                    \
	{                                                                         \
		return static_cast<T>(static_cast<int>(a) & static_cast<int>(b));     \
	}                                                                         \
	inline constexpr T operator^(T a, T b)                                    \
	{                                                                         \
		return static_cast<T>(static_cast<int>(a) ^ static_cast<int>(b));     \
	}                                                                         \
	inline constexpr T operator~(T e)                                         \
	{                                                                         \
		return static_cast<T>(~static_cast<int>(e));                          \
	}                                                                         \
	inline T operator|=(T a, T b)                                             \
	{                                                                         \
		return a = static_cast<T>(static_cast<int>(a) | static_cast<int>(b)); \
	}                                                                         \
	inline T operator&=(T a, T b)                                             \
	{                                                                         \
		return a = static_cast<T>(static_cast<int>(a) & static_cast<int>(b)); \
	}                                                                         \
	inline T operator^=(T a, T b)                                             \
	{                                                                         \
		return a = static_cast<T>(static_cast<int>(a) ^ static_cast<int>(b)); \
	}                                                                         \
	template <typename V>                                                     \
	inline bool operator==(V v, T e)                                          \
	{                                                                         \
		return v == static_cast<V>(e);                                        \
	}                                                                         \
	template <typename V>                                                     \
	inline bool operator==(T e, V v)                                          \
	{                                                                         \
		return v == e;                                                        \
	}                                                                         \
	template <typename V>                                                     \
	inline bool operator!=(T e, V v)                                          \
	{                                                                         \
		return !(v == e);                                                     \
	}                                                                         \
	template <typename V>                                                     \
	inline bool operator!=(V v, T e)                                          \
	{                                                                         \
		return !(v == e);                                                     \
	}

namespace Bitmask
{
	constexpr bool Test(uint8_t Value, uint8_t Mask)
	{
		return (Value & Mask) == Mask;
	}
	constexpr void Set(uint8_t Value, uint8_t Mask)
	{
		Value |= Mask;
	}
	constexpr void Reset(uint8_t Value, uint8_t Mask)
	{
		Value &= ~Mask;
	}
	constexpr bool IsValid(uint8_t Value)
	{
		return (Value != 0) && (Value & (Value - 1)) == 0;
	}
} // namespace Bitmask
