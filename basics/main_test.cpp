#include <gtest/gtest.h>
#include <string>

// Import functions from main.cpp
int add(int a, int b);
std::string greet(const std::string& name);

TEST(BasicTest, AddFunction) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
    EXPECT_EQ(add(0, 0), 0);
}

TEST(BasicTest, GreetFunction) {
    EXPECT_EQ(greet("World"), "Hello, World!");
    EXPECT_EQ(greet("Bazel"), "Hello, Bazel!");
} 