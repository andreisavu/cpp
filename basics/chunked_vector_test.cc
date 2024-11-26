#include "basics/chunked_vector.h"
#include <gtest/gtest.h>

namespace {

class ChunkedVectorTest : public ::testing::Test {
protected:
    static constexpr size_t kDefaultChunkSize = 3;
};

TEST_F(ChunkedVectorTest, InitializesWithChunkSize) {
    ChunkedVector<int> vec(kDefaultChunkSize);
    vec.push_back(1);
    EXPECT_EQ(vec[0], 1);
}

TEST_F(ChunkedVectorTest, PushBackCreatesNewChunks) {
    ChunkedVector<int> vec(2);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3); // This should create a new chunk
    
    // Test accessing elements across chunks
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(ChunkedVectorTest, OperatorAccessAcrossChunks) {
    ChunkedVector<int> vec(kDefaultChunkSize);
    
    // Fill multiple chunks
    for (int i = 0; i < 7; i++) {
        vec.push_back(i);
    }
    
    // Test accessing elements in different chunks
    EXPECT_EQ(vec[0], 0);  // First chunk
    EXPECT_EQ(vec[1], 1);  // First chunk
    EXPECT_EQ(vec[2], 2);  // First chunk
    EXPECT_EQ(vec[3], 3);  // Second chunk
    EXPECT_EQ(vec[4], 4);  // Second chunk
    EXPECT_EQ(vec[5], 5);  // Second chunk
    EXPECT_EQ(vec[6], 6);  // Third chunk
}

TEST_F(ChunkedVectorTest, WorksWithDifferentTypes) {
    ChunkedVector<std::string> vec(2);
    vec.push_back("hello");
    vec.push_back("world");
    vec.push_back("!");
    
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "world");
    EXPECT_EQ(vec[2], "!");
}

TEST_F(ChunkedVectorTest, HandlesLargeChunkSize) {
    constexpr size_t kLargeChunkSize = 1000;
    ChunkedVector<int> vec(kLargeChunkSize);
    
    // Fill first chunk
    for (size_t i = 0; i < kLargeChunkSize; i++) {
        vec.push_back(i);
    }
    
    // Add one more to create new chunk
    vec.push_back(kLargeChunkSize);
    
    // Verify values
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[kLargeChunkSize - 1], kLargeChunkSize - 1);
    EXPECT_EQ(vec[kLargeChunkSize], kLargeChunkSize);
}

} // namespace