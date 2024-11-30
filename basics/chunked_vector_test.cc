#include "basics/chunked_vector.h"
#include <gtest/gtest.h>

namespace {

class ChunkedVectorTest : public ::testing::Test {
protected:
    static constexpr size_t kDefaultChunkSize = 3;
};

TEST_F(ChunkedVectorTest, InitializesWithChunkSize) {
    ChunkedVector<int> vec(kDefaultChunkSize);
    vec.pushBack(1);
    EXPECT_EQ(vec[0], 1);
}

TEST_F(ChunkedVectorTest, PushBackCreatesNewChunks) {
    ChunkedVector<int> vec(2);
    vec.pushBack(1);
    vec.pushBack(2);
    vec.pushBack(3); // This should create a new chunk
    
    // Test accessing elements across chunks
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(ChunkedVectorTest, OperatorAccessAcrossChunks) {
    ChunkedVector<int> vec(kDefaultChunkSize);
    
    // Fill multiple chunks
    for (int i = 0; i < 7; i++) {
        vec.pushBack(i);
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
    vec.pushBack("hello");
    vec.pushBack("world");
    vec.pushBack("!");
    
    EXPECT_EQ(vec[0], "hello");
    EXPECT_EQ(vec[1], "world");
    EXPECT_EQ(vec[2], "!");
}

TEST_F(ChunkedVectorTest, HandlesLargeChunkSize) {
    constexpr size_t kLargeChunkSize = 1000;
    ChunkedVector<int> vec(kLargeChunkSize);
    
    // Fill first chunk
    for (size_t i = 0; i < kLargeChunkSize; i++) {
        vec.pushBack(i);
    }
    
    // Add one more to create new chunk
    vec.pushBack(kLargeChunkSize);
    
    // Verify values
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[kLargeChunkSize - 1], kLargeChunkSize - 1);
    EXPECT_EQ(vec[kLargeChunkSize], kLargeChunkSize);
}

TEST_F(ChunkedVectorTest, PackCombinesChunks) {
    ChunkedVector<int> vec(2);  // Small chunk size to force multiple chunks
    
    // Add elements to create multiple chunks
    for (int i = 0; i < 5; i++) {
        vec.pushBack(i);
    }
    
    // Pack the vector
    vec.pack();
    
    // Verify all elements are still accessible and in correct order
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(vec[i], i);
    }
}

TEST_F(ChunkedVectorTest, PackHandlesEmptyVector) {
    ChunkedVector<int> vec(3);
    vec.pack();  // Should not crash
}

TEST_F(ChunkedVectorTest, PackAllowsNewElements) {
    ChunkedVector<int> vec(2);
    
    // Add initial elements
    vec.pushBack(1);
    vec.pushBack(2);
    vec.pushBack(3);
    
    vec.pack();
    
    // Add more elements after packing
    vec.pushBack(4);
    vec.pushBack(5);
    
    // Verify all elements
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
    EXPECT_EQ(vec[3], 4);
    EXPECT_EQ(vec[4], 5);
}

TEST_F(ChunkedVectorTest, SplitCreatesNewChunks) {
    ChunkedVector<int> vec(2);
    vec.pushBack(1);
    vec.pushBack(2);
    vec.pushBack(3);
    vec.split(3);

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

} // namespace