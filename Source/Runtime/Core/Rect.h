#pragma once

#include <cstdint>

#include "Vector.h"

template <typename T>
class TRect
{
public:
	T X;
	T Y;
	T W;
	T H;

	TRect() = default;
	TRect(T inX, T inY, T inW, T inH) : X(inX), Y(inY), W(inW), H(inH) {}

	TVector4<T> ToVector4() const { return TVector4<T>(X, Y, W, H); }
	TVector2<T> Min() const { return TVector2<T>(X, Y); }
	TVector2<T> Max() const { return TVector2<T>(X + W, Y + H); }

	std::string ToString() const { return std::format("[[{}, {}], [{}, {}]]", X, Y, W, H); }
};

using FRect = TRect<float>;
using IRect = TRect<int32_t>;
