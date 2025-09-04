#include "Core/Logging.h"
#include "Core/CoreFwd.h"

#include "gtest/gtest.h"

TEST(VectorTestSuite, Vector2)
{
	FVector2 VecA(1.0f, 2.0f);
	FVector2 VecB(3.0f, 4.0f);

	EXPECT_EQ(VecA + VecB, FVector2(4.0f, 6.0f));
	EXPECT_EQ(VecA - VecB, FVector2(-2.0f, -2.0f));
	EXPECT_EQ(VecA * VecB, FVector2(3.0f, 8.0f));
	EXPECT_EQ(VecA / VecB, FVector2(1.0f / 3.0f, 2.0f / 4.0f));

	EXPECT_EQ(VecA + 1.0f, FVector2(2.0f, 3.0f));
	EXPECT_EQ(VecA - 1.0f, FVector2(0.0f, 1.0f));
	EXPECT_EQ(VecA * 2.0f, FVector2(2.0f, 4.0f));
	EXPECT_EQ(VecA / 2.0f, FVector2(0.5f, 1.0f));

	EXPECT_TRUE(VecA == FVector2(1.0f, 2.0f));
	EXPECT_TRUE(VecA != FVector2(1.0f, 3.0f));
}

TEST(VectorTestSuite, Vector3)
{
	FVector3 VecA(1.0f, 2.0f, 3.0f);
	FVector3 VecB(4.0f, 5.0f, 6.0f);

	EXPECT_EQ(VecA + VecB, FVector3(5.0f, 7.0f, 9.0f));
	EXPECT_EQ(VecA - VecB, FVector3(-3.0f, -3.0f, -3.0f));
	EXPECT_EQ(VecA * VecB, FVector3(4.0f, 10.0f, 18.0f));
	EXPECT_EQ(VecA / VecB, FVector3(1.0f / 4.0f, 2.0f / 5.0f, 3.0f / 6.0f));

	EXPECT_EQ(VecA + 1.0f, FVector3(2.0f, 3.0f, 4.0f));
	EXPECT_EQ(VecA - 1.0f, FVector3(0.0f, 1.0f, 2.0f));
	EXPECT_EQ(VecA * 2.0f, FVector3(2.0f, 4.0f, 6.0f));
	EXPECT_EQ(VecA / 2.0f, FVector3(0.5f, 1.0f, 1.5f));

	EXPECT_TRUE(VecA == FVector3(1.0f, 2.0f, 3.0f));
	EXPECT_TRUE(VecA != FVector3(1.0f, 2.0f, 4.0f));

	EXPECT_EQ(VecA.Cross(VecB), FVector3(-3.0f, 6.0f, -3.0f));
	EXPECT_EQ(VecA.Dot(VecB), 32.0f);
	EXPECT_EQ(VecA.Length(), std::sqrt(14.0f));
}

TEST(VectorTestSuite, Vector4)
{
	FVector4 VecA(1.0f, 2.0f, 3.0f, 4.0f);
	FVector4 VecB(5.0f, 6.0f, 7.0f, 8.0f);

	EXPECT_EQ(VecA + VecB, FVector4(6.0f, 8.0f, 10.0f, 12.0f));
	EXPECT_EQ(VecA - VecB, FVector4(-4.0f, -4.0f, -4.0f, -4.0f));
	EXPECT_EQ(VecA * VecB, FVector4(5.0f, 12.0f, 21.0f, 32.0f));
	EXPECT_EQ(VecA / VecB, FVector4(1.0f / 5.0f, 2.0f / 6.0f, 3.0f / 7.0f, 4.0f / 8.0f));

	EXPECT_EQ(VecA + 1.0f, FVector4(2.0f, 3.0f, 4.0f, 5.0f));
	EXPECT_EQ(VecA - 1.0f, FVector4(0.0f, 1.0f, 2.0f, 3.0f));
	EXPECT_EQ(VecA * 2.0f, FVector4(2.0f, 4.0f, 6.0f, 8.0f));
	EXPECT_EQ(VecA / 2.0f, FVector4(0.5f, 1.0f, 1.5f, 2.0f));

	EXPECT_TRUE(VecA == FVector4(1.0f, 2.0f, 3.0f, 4.0f));
	EXPECT_TRUE(VecA != FVector4(1.0f, 2.0f, 3.0f, 5.0f));
}
