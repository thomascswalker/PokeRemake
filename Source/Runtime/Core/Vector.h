#pragma once

#include "CFwd.h"
#include "Constants.h"
#include "Json.h"

template <typename T>
struct TVector2
{
	union
	{
		struct
		{
			T X, Y;
		};

		T XY[2];
	};

	// Constructors
	TVector2()
		: X(0), Y(0) {}

	TVector2(T InX, T InY)
		: X(InX), Y(InY) {}

	TVector2(const JSON& Json)
	{
		X = Json.at(0);
		Y = Json.at(1);
	}

	TVector2(const TVector2& Other) = default;
	TVector2(TVector2&& Other) noexcept = default;
	TVector2& operator=(const TVector2& Other) = default;
	TVector2& operator=(TVector2&& Other) noexcept = default;
	~TVector2() = default;

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
	T Dot(const TVector2& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}

	// Length
	T Length() const
	{
		return std::sqrt(X * X + Y * Y);
	}

	void Normalize()
	{
		auto L = Length();
		X /= L;
		Y /= L;
	}

	bool CloseEnough(const TVector2& Other, float Tolerance = EPSILON) const
	{
		return std::abs(X - Other.X) < Tolerance && std::abs(Y - Other.Y) < Tolerance;
	}

	std::string ToString() const
	{
		return std::format("[{}, {}]", X, Y);
	}

	JSON ToJson() const
	{
		JSON Result = JSON::array();
		Result.push_back(X);
		Result.push_back(Y);
		return Result;
	}

	static TVector2 Tile()
	{
		return { TILE_SIZE, TILE_SIZE };
	}

	static TVector2 Block()
	{
		return { BLOCK_SIZE, BLOCK_SIZE };
	}

	// Operators
	T& operator[](int32_t Index)
	{
		return XY[Index];
	}

	T operator[](int32_t Index) const
	{
		return XY[Index];
	}

	TVector2 operator+(const TVector2& Other) const
	{
		return { X + Other.X, Y + Other.Y };
	}

	TVector2 operator-(const TVector2& Other) const
	{
		return { X - Other.X, Y - Other.Y };
	}

	TVector2 operator*(const TVector2& Other) const
	{
		return { X * Other.X, Y * Other.Y };
	}

	TVector2 operator/(const TVector2& Other) const
	{
		return { X / Other.X, Y / Other.Y };
	}

	TVector2 operator+=(const TVector2& Other)
	{
		*this = *this + Other;
		return *this;
	}

	TVector2 operator-=(const TVector2& Other)
	{
		*this = *this - Other;
		return *this;
	}

	TVector2 operator*=(const TVector2& Other)
	{
		*this = *this * Other;
		return *this;
	}

	TVector2 operator/=(const TVector2& Other)
	{
		*this = *this / Other;
		return *this;
	}

	TVector2 operator+(const T Scalar) const
	{
		return { X + Scalar, Y + Scalar };
	}

	TVector2 operator-(const T Scalar) const
	{
		return { X - Scalar, Y - Scalar };
	}

	TVector2 operator*(const T Scalar) const
	{
		return { X * Scalar, Y * Scalar };
	}

	TVector2 operator/(const T Scalar) const
	{
		return { X / Scalar, Y / Scalar };
	}

	TVector2 operator+=(const T Scalar)
	{
		*this = *this + Scalar;
		return *this;
	}

	TVector2 operator-=(const T Scalar)
	{
		*this = *this - Scalar;
		return *this;
	}

	TVector2 operator*=(const T Scalar)
	{
		*this = *this * Scalar;
		return *this;
	}

	TVector2 operator/=(const T Scalar)
	{
		*this = *this / Scalar;
		return *this;
	}

	bool operator==(const TVector2& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const TVector2& Other) const
	{
		return !(*this == Other);
	}

	bool operator<(const TVector2& Other) const
	{
		return Other.X > X || Other.Y > Y;
	}

	bool operator>(const TVector2& Other) const
	{
		return Other.X < X || Other.Y < Y;
	}

	operator bool() const
	{
		return X != 0 || Y != 0;
	}

	friend std::ostream& operator<<(std::ostream& Stream, const TVector2& V)
	{
		Stream << "[" << V.X << ", " << V.Y << "]";
		return Stream;
	}
};

template <typename T>
struct TVector3
{
	union
	{
		struct
		{
			T X, Y, Z;
		};

		T XYZ[3];
	};

	TVector3()
		: X(0), Y(0), Z(0) {}

	TVector3(T InX, T InY, T InZ)
		: X(InX), Y(InY), Z(InZ) {}

	TVector3(const JSON& Json)
	{
		X = Json.array().at(0);
		Y = Json.array().at(1);
		Z = Json.array().at(2);
	}

	TVector3(const TVector3& Other) = default;
	TVector3(TVector3&& Other) noexcept = default;
	TVector3& operator=(const TVector3& Other) = default;
	TVector3& operator=(TVector3&& Other) noexcept = default;
	~TVector3() = default;

	bool operator==(const TVector3& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	bool operator!=(const TVector3& Other) const
	{
		return !(*this == Other);
	}

	TVector3 GetNormalized() const
	{
		T Length = this->Length();
		if (Length == 0)
		{
			return { 0, 0, 0 };
		}
		return { X / Length, Y / Length, Z / Length };
	}

	T Dot(const TVector3& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z;
	}

	TVector3 Cross(const TVector3& Other) const
	{
		return { Y * Other.Z - Z * Other.Y, Z * Other.X - X * Other.Z, X * Other.Y - Y * Other.X };
	}

	T Length() const
	{
		return std::sqrt(X * X + Y * Y + Z * Z);
	}

	std::string ToString() const
	{
		return std::format("[{}, {}, {}]", X, Y, Z);
	}

	JSON ToJson() const
	{
		JSON Result = JSON::array();
		Result.push_back(X);
		Result.push_back(Y);
		Result.push_back(Z);
		return Result;
	}

	T& operator[](int32_t Index)
	{
		return XYZ[Index];
	}

	T operator[](int32_t Index) const
	{
		return XYZ[Index];
	}

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

	TVector3 operator+=(const TVector3& Other)
	{
		return *this + Other;
	}

	TVector3 operator-=(const TVector3& Other)
	{
		return *this - Other;
	}

	TVector3 operator*=(const TVector3& Other)
	{
		return *this * Other;
	}

	TVector3 operator/=(const TVector3& Other)
	{
		return *this / Other;
	}

	TVector3 operator+(const T Scalar) const
	{
		return { X + Scalar, Y + Scalar, Z + Scalar };
	}

	TVector3 operator-(const T Scalar) const
	{
		return { X - Scalar, Y - Scalar, Z - Scalar };
	}

	TVector3 operator*(const T Scalar) const
	{
		return { X * Scalar, Y * Scalar, Z * Scalar };
	}

	TVector3 operator/(const T Scalar) const
	{
		return { X / Scalar, Y / Scalar, Z / Scalar };
	}

	TVector3 operator+=(const T Scalar)
	{
		return *this + Scalar;
	}

	TVector3 operator-=(const T Scalar)
	{
		return *this - Scalar;
	}

	TVector3 operator*=(const T Scalar)
	{
		return *this * Scalar;
	}

	TVector3 operator/=(const T Scalar)
	{
		return *this / Scalar;
	}

	friend std::ostream& operator<<(std::ostream& Stream, const TVector3& V)
	{
		Stream << "[" << V.X << ", " << V.Y << ", " << V.Z << "]";
		return Stream;
	}
};

template <typename T>
struct TVector4
{
	union
	{
		struct
		{
			T X, Y, Z, W;
		};

		T XYZW[4];
	};

	TVector4()
		: X(0), Y(0), Z(0), W(0) {}

	TVector4(T InX, T InY, T InZ, T InW)
		: X(InX), Y(InY), Z(InZ), W(InW) {}

	TVector4(const JSON& Json)
	{
		X = Json.array().at(0);
		Y = Json.array().at(1);
		Z = Json.array().at(2);
		W = Json.array().at(3);
	}

	TVector4(const TVector4& Other) = default;
	TVector4(TVector4&& Other) noexcept = default;
	TVector4& operator=(const TVector4& Other) = default;
	TVector4& operator=(TVector4&& Other) noexcept = default;
	~TVector4() = default;

	bool operator==(const TVector4& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
	}

	bool operator!=(const TVector4& Other) const
	{
		return !(*this == Other);
	}

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

	T Length() const
	{
		return std::sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	std::string ToString() const
	{
		return std::format("[{:.3f}, {:.3f}, {:.3f}, {:.3f}]", X, Y, Z, W);
	}

	JSON ToJson() const
	{
		JSON Result = JSON::array();
		Result.push_back(X);
		Result.push_back(Y);
		Result.push_back(Z);
		Result.push_back(W);
		return Result;
	}

	T& operator[](int32_t Index)
	{
		return XYZW[Index];
	}

	T operator[](int32_t Index) const
	{
		return XYZW[Index];
	}

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

	TVector4 operator+=(const TVector4& Other)
	{
		return *this + Other;
	}

	TVector4 operator-=(const TVector4& Other)
	{
		return *this - Other;
	}

	TVector4 operator*=(const TVector4& Other)
	{
		return *this * Other;
	}

	TVector4 operator/=(const TVector4& Other)
	{
		return *this / Other;
	}

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

	TVector4 operator+=(const T Scalar)
	{
		return *this + Scalar;
	}

	TVector4 operator-=(const T Scalar)
	{
		return *this - Scalar;
	}

	TVector4 operator*=(const T Scalar)
	{
		return *this * Scalar;
	}

	TVector4 operator/=(const T Scalar)
	{
		return *this / Scalar;
	}

	friend std::ostream& operator<<(std::ostream& Stream, const TVector4& V)
	{
		Stream << "[" << V.X << ", " << V.Y << ", " << V.Z << ", " << V.W << "]";
		return Stream;
	}
};
