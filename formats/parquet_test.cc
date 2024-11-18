#include <gtest/gtest.h>
#include "parquet.h"

TEST(BasicTest, AtomicTypeEnum)
{
    AtomicType t = AtomicType::INT32;
    EXPECT_NE(t, AtomicType::BOOLEAN);
    EXPECT_EQ(t, AtomicType::INT32);
}
