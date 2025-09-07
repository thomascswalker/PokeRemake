#pragma once

#include "Decl.h"

// Row-major 4x4 Matrix
template <typename T>
struct TMatrix
{
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

	std::string ToString() const
	{
		return std::format("{}\n{}\n{}\n{}", M[0].ToString().c_str(), M[1].ToString().c_str(),
						   M[2].ToString().c_str(), M[3].ToString().c_str());
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

	TVector4<T> operator*(const TVector4<T>& Vec) const
	{
		return TVector4<T>(M[0].X * Vec.X + M[0].Y * Vec.Y + M[0].Z * Vec.Z + M[0].W * Vec.W,
						   M[1].X * Vec.X + M[1].Y * Vec.Y + M[1].Z * Vec.Z + M[1].W * Vec.W,
						   M[2].X * Vec.X + M[2].Y * Vec.Y + M[2].Z * Vec.Z + M[2].W * Vec.W,
						   M[3].X * Vec.X + M[3].Y * Vec.Y + M[3].Z * Vec.Z + M[3].W * Vec.W);
	}

	friend std::ostream& operator<<(std::ostream& Stream, const TMatrix& Matrix)
	{
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			Stream << Matrix.M[Row] << std::endl;
		}
		return Stream;
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