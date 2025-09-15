#pragma once
#include <initializer_list>

template <typename T>
struct TBounds
{
	T Left;
	T Bottom;
	T Right;
	T Top;

	TBounds() : Left(0), Bottom(0), Right(0), Top(0) {}

	explicit TBounds(T Value)
		: Left(Value), Bottom(Value), Right(Value), Top(Value) {}

	explicit TBounds(T InLeft, T InBottom, T InRight, T InTop)
		: Left(InLeft), Bottom(InBottom), Right(InRight), Top(InTop) {}

	TBounds(std::initializer_list<T> Values)
	{
		T* P = &Left;
		for (auto it = Values.begin(); it != Values.end(); ++it)
		{
			*P++ = *it;
		}
	}

	T X() const { return Left + Right; }
	T Y() const { return Bottom + Top; }

	TBounds& operator=(T Value)
	{
		Left = Value;
		Bottom = Value;
		Right = Value;
		Top = Value;
		return *this;
	}
};

using FBounds = TBounds<float>;
