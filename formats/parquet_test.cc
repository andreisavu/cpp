#include <gtest/gtest.h>
#include "parquet.h"

TEST(BasicTest, PrimitiveTypeEnum)
{
    PrimitiveType t = PrimitiveType::INT32;
    EXPECT_NE(t, PrimitiveType::BOOLEAN);
    EXPECT_EQ(t, PrimitiveType::INT32);
}
