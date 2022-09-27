#include <gtest/gtest.h>

namespace Hzn
{
	TEST(SampleTest, TrueTest)
	{
		EXPECT_TRUE(true);
	}

	TEST(SampleTest, AssertTest)
	{
		EXPECT_EQ(42, 42);
	}
}