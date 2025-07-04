#pragma once

#include <cstdint>

#include "SDL3/SDL.h"
#include "Vector.h"

template <typename T>
class TRect
{
public:
	T X;
	T Y;
	T W;
	T H;

	TRect()
		: X(0), Y(0), W(0), H(0) {}
	TRect(T inX, T inY, T inW, T inH)
		: X(inX), Y(inY), W(inW), H(inH) {}

	TVector4<T> ToVector4() const { return TVector4<T>(X, Y, W, H); }
	TVector2<T> Min() const { return TVector2<T>(X, Y); }
	TVector2<T> Max() const { return TVector2<T>(X + W, Y + H); }
	TVector2<T> GetSize() const { return TVector2<T>(W, H); }

	bool Contains(const TVector2<T>& Point) const
	{
		return Point.X >= X && Point.X < X + W && Point.Y >= Y && Point.Y < Y + H;
	}
	bool Contains(const TRect& Other) const
	{
		return Other.X >= X && Other.X + Other.W < X + W && Other.Y >= Y
			   && Other.Y + Other.H < Y + H;
	}
	bool Intersects(const TRect& Other) const
	{
		return !(Other.X > X + W || Other.X + Other.W < X || Other.Y > Y + H
				 || Other.Y + Other.H < Y);
	}
	bool Overlaps(const TRect& Other) const
	{
		return !(Other.X >= X + W || Other.X + Other.W <= X || Other.Y >= Y + H
				 || Other.Y + Other.H <= Y);
	}
	bool operator==(const TRect& Other) const
	{
		return X == Other.X && Y == Other.Y && W == Other.W && H == Other.H;
	}
	bool operator!=(const TRect& Other) const { return !(*this == Other); }

	std::string ToString() const { return std::format("[[{}, {}], [{}, {}]]", X, Y, W, H); }

	void Expand(T Size)
	{
		X -= Size * 2;
		Y -= Size * 2;
		W += Size * 2;
		H += Size * 2;
	}
	TRect Expanded(T Size) const
	{
		TRect Out = *this;
		Out.Expand(Size);
		return Out;
	}

	SDL_FRect ToSDL_FRect() const
	{
		return { (float)X, (float)Y, (float)W, (float)H };
	}

	SDL_Rect ToSDL_Rect() const
	{
		return { (int)X, (int)Y, (int)W, (int)H };
	}
};

using FRect = TRect<float>;
using IRect = TRect<int32_t>;
