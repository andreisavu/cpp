#include <gtest/gtest.h>
#include "parquet.h"

TEST(BasicTest, GreetFunction)
{
    EXPECT_EQ(greet("Parquet"), "Hello, Parquet!");
}