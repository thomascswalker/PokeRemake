#define GLM_ENABLE_EXPERIMENTAL

#include "Core/Logging.h"
#include "Core/Matrix.h"
#include "gtest/gtest.h"

#define FLOAT_THRESHOLD 0.001f

TEST(MatrixTestSuite, TestMatrix)
{
	const FMatrix M0({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 });
	const FMatrix M1({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 });
	const FMatrix M2({ 16, 15, 14, 13 }, { 12, 11, 10, 9 }, { 8, 7, 6, 5 }, { 4, 3, 2, 1 });

	// Equality
	LogDebug("Testing matrix equality");
	ASSERT_TRUE(M0 == M1);
	ASSERT_TRUE(M0 != M2);

	// Addition
	LogDebug("Testing matrix addition");
	ASSERT_EQ(M0 + M1,
			  FMatrix({ 2, 4, 6, 8 }, { 10, 12, 14, 16 }, { 18, 20, 22, 24 }, { 26, 28, 30, 32 }));

	LogDebug("Testing matrix multiplication");
	// Multiplication
	ASSERT_EQ(M0 * M1, FMatrix({ 90, 100, 110, 120 }, { 202, 228, 254, 280 },
							   { 314, 356, 398, 440 }, { 426, 484, 542, 600 }));

	LogDebug("Testing matrix transposition");
	// Transposition
	ASSERT_EQ(M0.GetTransposed(),
			  FMatrix({ 1, 5, 9, 13 }, { 2, 6, 10, 14 }, { 3, 7, 11, 15 }, { 4, 8, 12, 16 }));

	// Matrix * Vector multiplication
	LogDebug("Testing matrix-vector multiplication");
	FVector4 Vec(1, 2, 3, 4);
	FVector4 Result = M0 * Vec;
	ASSERT_EQ(Result, FVector4(30, 70, 110, 150));
}

TEST(MatrixTestSuite, TestPerspective)
{
	LogDebug("Testing matrix perspective projection");

	const float FovY = 90.0f;
	const float Aspect = 16.0f / 9.0f; // Assuming square viewport
	const float Near = 0.1f;
	const float Far = 100.0f;

	FMatrix m0({ 0.347270f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.617370f, 0.0f, 0.0f },
			   { 0.0f, 0.0f, -1.002002f, -0.200200f }, { 0.0f, 0.0f, -1.0f, 0.0f });
	FMatrix m1 = MakePerspectiveMatrix(FovY, Aspect, Near, Far);

	for (int32_t Row = 0; Row < 4; ++Row)
	{
		for (int32_t Col = 0; Col < 4; ++Col)
		{
			EXPECT_NEAR(m0[Row][Col], m1[Row][Col], FLOAT_THRESHOLD)
				<< "Mismatch at Row: " << Row << ", Col: " << Col << " - Expected: " << m0[Row][Col]
				<< ", Got: " << m1[Row][Col];
		}
		std::cout << std::endl;
	}
}

TEST(MatrixTestSuite, TestOrtho)
{
	LogDebug("Testing matrix orthographic projection.");

	const float Left = 0.0f;
	const float Right = 640.0f;
	const float Bottom = 480.0f;
	const float Top = 0.0f;
	const float Near = 0.1f;
	const float Far = 100.0f;

	auto m0 = MakeOrthographicMatrix(Left, Right, Bottom, Top, Near, Far);
	auto m1 = FMatrix({ 0.003f, 0.0f, 0.0f, 0.0f },		   // Row 1
					  { 0.0f, -0.004f, 0.0f, 0.0f },	   // Row 2
					  { 0.0f, 0.0f, -0.020f, 0.0f },	   // Row 3
					  { -1.000f, 1.000f, -1.002f, 1.000f } // Row 4
	);

	for (int32_t Row = 0; Row < 4; ++Row)
	{
		for (int32_t Col = 0; Col < 4; ++Col)
		{
			EXPECT_NEAR(m0[Row][Col], m1[Row][Col], FLOAT_THRESHOLD)
				<< "Mismatch at Row: " << Row << ", Col: " << Col << " - Expected: " << m0[Row][Col]
				<< ", Got: " << m1[Row][Col];
		}
		std::cout << std::endl;
	}
}

TEST(MatrixTestSuite, TestLookAt)
{
	LogDebug("Testing matrix look-at");

	const FVector3 Eye(5.0f, 5.0f, 5.0f);
	const FVector3 Center(0.0f, 0.0f, 0.0f);
	const FVector3 Up(0.0f, 1.0f, 0.0f);

	FMatrix m0 = MakeLookAtMatrix(Eye, Center, Up);
	FMatrix m1({ 0.707107f, -0.408248f, 0.57735f, 0.0f }, { 0.0f, 0.816497f, 0.57735f, 0.0f },
			   { -0.707107f, -0.408248f, 0.57735f, 0.0f }, { 0.0f, 0.0f, -8.66025f, 1.0f });

	for (auto Row = 0; Row < 4; ++Row)
	{
		for (auto Col = 0; Col < 4; ++Col)
		{
			EXPECT_NEAR(m0[Row][Col], m1[Row][Col], FLOAT_THRESHOLD)
				<< "Mismatch at Row: " << Row << ", Col: " << Col << " - Expected: " << m0[Row][Col]
				<< ", Got: " << m1[Row][Col];
		}
		std::cout << std::endl;
	}
}