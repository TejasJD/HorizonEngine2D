#include <gtest/gtest.h>
#include "HorizonEngine/Physics2D/RigidBody2D.h"

namespace Hzn {
TEST(SampleTest, TrueTest) { EXPECT_TRUE(true); }

TEST(SampleTest, AssertTest) { EXPECT_EQ(42, 42); }
}  // namespace Hzn