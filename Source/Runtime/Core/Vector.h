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
	union
	{
		struct
		{
			T X, Y;
		};
		T XY[2];
	};

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

	TVector2 GetNormalized() const
	{
		T Length = this->Length();
		if (Length == 0)
		{
			return { 0, 0 };
		}
		return { X / Length, Y / Length };
	}

	// Dot product
	T Dot(const TVector2& Other) const { return X * Other.X + Y * Other.Y; }

	// Length
	T Length() const { return std::sqrt(X * X + Y * Y); }

	// Operators
	T& operator[](int32_t Index) { return XY[Index]; }
	T  operator[](int32_t Index) const { return XY[Index]; }

	TVector2 operator+(const TVector2& Other) const { return { X + Other.X, Y + Other.Y }; }
	TVector2 operator-(const TVector2& Other) const { return { X - Other.X, Y - Other.Y }; }
	TVector2 operator*(const TVector2& Other) const { return { X * Other.X, Y * Other.Y }; }
	TVector2 operator/(const TVector2& Other) const { return { X / Other.X, Y / Other.Y }; }
	TVector2 operator+=(const TVector2& Other) { return *this + Other; }
	TVector2 operator-=(const TVector2& Other) { return *this - Other; }
	TVector2 operator*=(const TVector2& Other) { return *this * Other; }
	TVector2 operator/=(const TVector2& Other) { return *this / Other; }

	TVector2 operator+(const T Scalar) const { return { X + Scalar, Y + Scalar }; }
	TVector2 operator-(const T Scalar) const { return { X - Scalar, Y - Scalar }; }
	TVector2 operator*(const T Scalar) const { return { X * Scalar, Y * Scalar }; }
	TVector2 operator/(const T Scalar) const { return { X / Scalar, Y / Scalar }; }
	TVector2 operator+=(const T Scalar) { return *this + Scalar; }
	TVector2 operator-=(const T Scalar) { return *this - Scalar; }
	TVector2 operator*=(const T Scalar) { return *this * Scalar; }
	TVector2 operator/=(const T Scalar) { return *this / Scalar; }
};

template <typename T>
class TVector3
{
public:
	union
	{
		struct
		{
			T X, Y, Z;
		};
		T XYZ[3];
	};

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

	TVector3 GetNormalized() const
	{
		T Length = this->Length();
		if (Length == 0)
		{
			return { 0, 0, 0 };
		}
		return { X / Length, Y / Length, Z / Length };
	}

	T Dot(const TVector3& Other) const { return X * Other.X + Y * Other.Y + Z * Other.Z; }

	TVector3 Cross(const TVector3& Other) const
	{
		return { Y * Other.Z - Z * Other.Y, Z * Other.X - X * Other.Z, X * Other.Y - Y * Other.X };
	}

	T Length() const { return std::sqrt(X * X + Y * Y + Z * Z); }

	T& operator[](int32_t Index) { return XYZ[Index]; }
	T  operator[](int32_t Index) const { return XYZ[Index]; }

	TVector3 operator+(const TVector3& Other) const
	{
		return { X + Other.X, Y + Other.Y, Z + Other.Z };
	}
	TVector3 operator-(const TVector3& Other) const
	{
		return { X - Other.X, Y - Other.Y, Z - Other.Z };
	}
	TVector3 operator*(const TVector3& Other) const
	{
		return { X * Other.X, Y * Other.Y, Z * Other.Z };
	}
	TVector3 operator/(const TVector3& Other) const
	{
		return { X / Other.X, Y / Other.Y, Z / Other.Z };
	}
	TVector3 operator+=(const TVector3& Other) { return *this + Other; }
	TVector3 operator-=(const TVector3& Other) { return *this - Other; }
	TVector3 operator*=(const TVector3& Other) { return *this * Other; }
	TVector3 operator/=(const TVector3& Other) { return *this / Other; }

	TVector3 operator+(const T Scalar) const { return { X + Scalar, Y + Scalar, Z + Scalar }; }
	TVector3 operator-(const T Scalar) const { return { X - Scalar, Y - Scalar, Z - Scalar }; }
	TVector3 operator*(const T Scalar) const { return { X * Scalar, Y * Scalar, Z * Scalar }; }
	TVector3 operator/(const T Scalar) const { return { X / Scalar, Y / Scalar, Z / Scalar }; }
	TVector3 operator+=(const T Scalar) { return *this + Scalar; }
	TVector3 operator-=(const T Scalar) { return *this - Scalar; }
	TVector3 operator*=(const T Scalar) { return *this * Scalar; }
	TVector3 operator/=(const T Scalar) { return *this / Scalar; }
};

template <typename T>
class TVector4
{
public:
	union
	{
		struct
		{
			T X, Y, Z, W;
		};
		T XYZW[4];
	};

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

	TVector4 GetNormalized() const
	{
		T Length = this->Length();
		if (Length == 0)
		{
			return { 0, 0, 0, 0 };
		}
		return { X / Length, Y / Length, Z / Length, W / Length };
	}

	T Dot(const TVector4& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
	}

	T Length() const { return std::sqrt(X * X + Y * Y + Z * Z + W * W); }

	T& operator[](int32_t Index) { return XYZW[Index]; }
	T  operator[](int32_t Index) const { return XYZW[Index]; }

	TVector4 operator+(const TVector4& Other) const
	{
		return TVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
	}
	TVector4 operator-(const TVector4& Other) const
	{
		return TVector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
	}
	TVector4 operator*(const TVector4& Other) const
	{
		return { X * Other.X, Y * Other.Y, Z * Other.Z, W * Other.W };
	}
	TVector4 operator/(const TVector4& Other) const
	{
		return { X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W };
	}
	TVector4 operator+=(const TVector4& Other) { return *this + Other; }
	TVector4 operator-=(const TVector4& Other) { return *this - Other; }
	TVector4 operator*=(const TVector4& Other) { return *this * Other; }
	TVector4 operator/=(const TVector4& Other) { return *this / Other; }

	TVector4 operator+(const T Scalar) const
	{
		return { X + Scalar, Y + Scalar, Z + Scalar, W + Scalar };
	}
	TVector4 operator-(const T Scalar) const
	{
		return { X - Scalar, Y - Scalar, Z - Scalar, W - Scalar };
	}
	TVector4 operator*(const T Scalar) const
	{
		return { X * Scalar, Y * Scalar, Z * Scalar, W * Scalar };
	}
	TVector4 operator/(const T Scalar) const
	{
		return { X / Scalar, Y / Scalar, Z / Scalar, W / Scalar };
	}
	TVector4 operator+=(const T Scalar) { return *this + Scalar; }
	TVector4 operator-=(const T Scalar) { return *this - Scalar; }
	TVector4 operator*=(const T Scalar) { return *this * Scalar; }
	TVector4 operator/=(const T Scalar) { return *this / Scalar; }
};