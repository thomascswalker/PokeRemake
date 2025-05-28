#pragma once

#include "Vector.h"

// Row-major 4x4 Matrix
template <typename T>
class TMatrix
{
public:
	TVector4<T> M[4];

	// Constructors
	TMatrix()
		: M{ TVector4<T>(1, 0, 0, 0), TVector4<T>(0, 1, 0, 0), TVector4<T>(0, 0, 1, 0),
			  TVector4<T>(0, 0, 0, 1) }
	{
	}

	TMatrix(const TVector4<T>& Row0, const TVector4<T>& Row1, const TVector4<T>& Row2,
		const TVector4<T>& Row3)
		: M{ Row0, Row1, Row2, Row3 }
	{
	}

	// Element access
	const TVector4<T>& operator[](int32_t Row) const { return M[Row]; }
	TVector4<T>&	   operator[](int32_t Row) { return M[Row]; }

	// Static identity
	static TMatrix Identity() { return TMatrix(); }

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

	// Matrix multiplication
	TMatrix operator*(const TMatrix& Other) const
	{
		TMatrix Result;
		for (int32_t Row = 0; Row < 4; ++Row)
		{
			for (int32_t Col = 0; Col < 4; ++Col)
			{
				Result.M[Row][Col] = M[Row].X * Other.M[0][Col] + M[Row].Y * Other.M[1][Col]
					+ M[Row].Z * Other.M[2][Col] + M[Row].W * Other.M[3][Col];
			}
		}
		return Result;
	}

	// Matrix-vector multiplication
	TVector4<T> operator*(const TVector4<T>& Vec) const
	{
		return { M[0].Dot(Vec), M[1].Dot(Vec), M[2].Dot(Vec), M[3].Dot(Vec) };
	}
};