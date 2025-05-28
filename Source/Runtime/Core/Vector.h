#pragma once

#include <cmath>
#include <cstdint>

template <typename T>
class TVector2;
template <typename T>
class TVector3;
template <typename T>
class TVector4;

using FVector2 = TVector2<float>;
using FVector3 = TVector3<float>;
using FVector4 = TVector4<float>;
using IVector2 = TVector2<int32_t>;
using IVector3 = TVector3<int32_t>;
using IVector4 = TVector4<int32_t>;

template <typename T>
class TVector2
{
public:
	T X, Y;

	// Constructors
	TVector2() : X(0), Y(0) {}
	TVector2(T InX, T InY) : X(InX), Y(InY) {}
	TVector2(const TVector2& Other) = default;
	TVector2(TVector2&& Other) noexcept = default;
	TVector2& operator=(const TVector2& Other) = default;
	TVector2& operator=(TVector2&& Other) noexcept = default;
	~TVector2() = default;

	// Equality
	bool operator==(const TVector2& Other) const { return X == Other.X && Y == Other.Y; }
	bool operator!=(const TVector2& Other) const { return !(*this == Other); }

	// Dot product
	T Dot(const TVector2& Other) const { return X * Other.X + Y * Other.Y; }

	// Length
	T Length() const { return std::sqrt(X * X + Y * Y); }

	// Arithmetic
	TVector2 operator+(const TVector2& Other) const { return { X + Other.X, Y + Other.Y }; }
	TVector2 operator-(const TVector2& Other) const { return { X - Other.X, Y - Other.Y }; }
	TVector2 operator*(T Scalar) const { return { X * Scalar, Y * Scalar }; }
	TVector2 operator/(T Scalar) const { return { X / Scalar, Y / Scalar }; }
};

template <typename T>
class TVector3
{
public:
	T X, Y, Z;

	TVector3() : X(0), Y(0), Z(0) {}
	TVector3(T InX, T InY, T InZ) : X(InX), Y(InY), Z(InZ) {}
	TVector3(const TVector3& Other) = default;
	TVector3(TVector3&& Other) noexcept = default;
	TVector3& operator=(const TVector3& Other) = default;
	TVector3& operator=(TVector3&& Other) noexcept = default;
	~TVector3() = default;

	bool operator==(const TVector3& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}
	bool operator!=(const TVector3& Other) const { return !(*this == Other); }

	T Dot(const TVector3& Other) const { return X * Other.X + Y * Other.Y + Z * Other.Z; }

	TVector3 Cross(const TVector3& Other) const
	{
		return { Y * Other.Z - Z * Other.Y, Z * Other.X - X * Other.Z, X * Other.Y - Y * Other.X };
	}

	T Length() const { return std::sqrt(X * X + Y * Y + Z * Z); }

	TVector3 operator+(const TVector3& Other) const
	{
		return TVector3(X + Other.X, Y + Other.Y, Z + Other.Z);
	}
	TVector3 operator-(const TVector3& Other) const
	{
		return TVector3(X - Other.X, Y - Other.Y, Z - Other.Z);
	}
	TVector3 operator*(T Scalar) const { return { X * Scalar, Y * Scalar, Z * Scalar }; }
	TVector3 operator/(T Scalar) const { return { X / Scalar, Y / Scalar, Z / Scalar }; }
};

template <typename T>
class TVector4
{
public:
	T X, Y, Z, W;

	TVector4() : X(0), Y(0), Z(0), W(0) {}
	TVector4(T InX, T InY, T InZ, T InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
	TVector4(const TVector4& Other) = default;
	TVector4(TVector4&& Other) noexcept = default;
	TVector4& operator=(const TVector4& Other) = default;
	TVector4& operator=(TVector4&& Other) noexcept = default;
	~TVector4() = default;

	bool operator==(const TVector4& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
	}
	bool operator!=(const TVector4& Other) const { return !(*this == Other); }

	T Dot(const TVector4& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
	}

	T Length() const { return std::sqrt(X * X + Y * Y + Z * Z + W * W); }

	TVector4 operator+(const TVector4& Other) const
	{
		return { X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W };
	}
	TVector4 operator-(const TVector4& Other) const
	{
		return { X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W };
	}
	TVector4 operator*(T Scalar) const
	{
		return { X * Scalar, Y * Scalar, Z * Scalar, W * Scalar };
	}
	TVector4 operator/(T Scalar) const
	{
		return { X / Scalar, Y / Scalar, Z / Scalar, W / Scalar };
	}
};