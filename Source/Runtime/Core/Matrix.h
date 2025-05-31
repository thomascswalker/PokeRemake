#pragma once

#include "Vector.h"

template <typename T>
class TMatrix;
using FMatrix = TMatrix<float>;
using DMatrix = TMatrix<double>;
using IMatrix = TMatrix<int32_t>;

// Row-major 4x4 Matrix
template <typename T>
class TMatrix
{
public:
	TVector4<T> M[4];

	// Constructors
	TMatrix()
		: M{ TVector4<T>(0, 0, 0, 0), TVector4<T>(0, 0, 0, 0), TVector4<T>(0, 0, 0, 0),
			 TVector4<T>(0, 0, 0, 0) }
	{
	}

	TMatrix(const T Value)
		: M{ TVector4<T>(Value, 0, 0, 0), TVector4<T>(0, Value, 0, 0), TVector4<T>(0, 0, Value, 0),
			 TVector4<T>(0, 0, 0, Value) }
	{
	}

	TMatrix(const TVector4<T>& Row0, const TVector4<T>& Row1, const TVector4<T>& Row2,
			const TVector4<T>& Row3)
		: M{ Row0, Row1, Row2, Row3 }
	{
	}
	// Static identity
	static TMatrix Identity() { return TMatrix(1); }

	// Transpose
	TMatrix GetTransposed() const
	{
		TMatrix Result;
		for (int32_t i = 0; i < 4; ++i)
		{
			for (int32_t j = 0; j < 4; ++j)
			{
				Result.M[i][j] = M[j][i];
			}
		}
		return Result;
	}

	/* Operators */

	bool operator==(const TMatrix& Other) const
	{
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			if (M[Row] != Other.M[Row])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const TMatrix& Other) const { return !(*this == Other); }

	const TVector4<T>& operator[](int32_t Row) const { return M[Row]; }
	TVector4<T>&	   operator[](int32_t Row) { return M[Row]; }

	TMatrix operator+(const TMatrix& Other) const
	{
		TMatrix Result;
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			Result.M[Row] = M[Row] + Other.M[Row];
		}
		return Result;
	}

	TMatrix operator-(const TMatrix& Other) const
	{
		TMatrix Result;
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			Result.M[Row] = M[Row] - Other.M[Row];
		}
		return Result;
	}

	// Matrix multiplication
	TMatrix operator*(const TMatrix& Other) const
	{
		TMatrix Result;
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			for (int32_t Col = 0; Col < 4; ++Col)
			{
				Result[Row][Col] = M[Row].X * Other[0][Col] + M[Row].Y * Other[1][Col]
								   + M[Row].Z * Other[2][Col] + M[Row].W * Other[3][Col];
			}
		}
		return Result;
	}
};

template <typename T>
TMatrix<T> MakePerspectiveMatrix(T FovY, T Aspect, T Near, T Far)
{
	TMatrix<T> Result(0);
	T		   TanHalfFov = std::tan(FovY / static_cast<T>(2));

	Result[0][0] = static_cast<T>(1) / (Aspect * TanHalfFov);
	Result[1][1] = static_cast<T>(1) / TanHalfFov;
	Result[2][2] = -(Far + Near) / (Far - Near);
	Result[3][2] = -static_cast<T>(1);
	Result[2][3] = -(static_cast<T>(2) * Far * Near) / (Far - Near);
	Result[3][3] = 0;

	return Result;
}

template <typename T>
TMatrix<T> MakeOrthographicMatrix(T Left, T Right, T Bottom, T Top, T Near, T Far)
{
	TMatrix<T> Result(1);

	Result[0][0] = static_cast<T>(2) / (Right - Left);
	Result[1][1] = static_cast<T>(2) / (Top - Bottom);
	Result[2][2] = -static_cast<T>(2) / (Far - Near);
	Result[3][0] = -(Right + Left) / (Right - Left);
	Result[3][1] = -(Top + Bottom) / (Top - Bottom);
	Result[3][2] = -(Far + Near) / (Far - Near);

	return Result;
}

template <typename T>
TMatrix<T> MakeLookAtMatrix(const TVector3<T>& Eye, const TVector3<T>& Target,
							const TVector3<T>& Up)
{
	TVector3<T> Forward = (Target - Eye).GetNormalized();
	TVector3<T> Right = Forward.Cross(Up).GetNormalized();
	TVector3<T> UpVector = Right.Cross(Forward).GetNormalized();

	TMatrix<T> Result(1);
	Result[0] = TVector4<T>(Right.X, UpVector.X, -Forward.X, 0);
	Result[1] = TVector4<T>(Right.Y, UpVector.Y, -Forward.Y, 0);
	Result[2] = TVector4<T>(Right.Z, UpVector.Z, -Forward.Z, 0);
	Result[3] = TVector4<T>(-Right.Dot(Eye), -UpVector.Dot(Eye), Forward.Dot(Eye), 1);
	return Result;
}