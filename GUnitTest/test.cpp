#include "pch.h"
#include "src/foo.cpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(FooTest, Foo)
{
	FOO f = FOO();
	f.add(2);
	f.subtract(1);
	EXPECT_EQ(1, f.getVal());
}