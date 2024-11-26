#pragma once

#include <vector>

/**
 * Similar to std::vector, but with a chunk size. Adding elements to the end of the vector will
 * create a new chunk if the current chunk is full and add the element to the new chunk.
 */
template <typename T>
class ChunkedVector
{
public:
    ChunkedVector(size_t chunk_size) : chunk_size_(chunk_size) 
    {
        // Start with one chunk
        chunks_.push_back(std::vector<T>());
        chunks_.back().reserve(chunk_size);
    }

    void pushBack(const T &value)
    {
        // If current chunk is full, create a new one
        if (chunks_.back().size() == chunk_size_)
        {
            chunks_.push_back(std::vector<T>());
            chunks_.back().reserve(chunk_size_);
        }
        
        // Add the value to the current chunk
        chunks_.back().push_back(value);
    }

    T &operator[](size_t index)
    {
        size_t total_size = 0;
        for (const auto& chunk : chunks_) {
            total_size += chunk.size();
        }
        
        if (index >= total_size) {
            throw std::out_of_range("Index out of bounds");
        }

        size_t chunk_index = index / chunk_size_;
        size_t element_index = index % chunk_size_;
        return chunks_[chunk_index][element_index];
    }

    // Combines all chunks into a single chunk and adjusts the chunk size
    void pack()
    {
        // Calculate total size
        size_t total_size = 0;
        for (const auto& chunk : chunks_) {
            total_size += chunk.size();
        }

        if (total_size > 0) {
            // Reuse first chunk's memory instead of creating new vector
            auto& first_chunk = chunks_[0];
            first_chunk.reserve(total_size);

            // Copy elements from other chunks into first chunk and clear them
            for (size_t i = 1; i < chunks_.size(); i++) {
                first_chunk.insert(first_chunk.end(), 
                                 chunks_[i].begin(), 
                                 chunks_[i].end());
                chunks_[i].clear();
                chunks_[i].shrink_to_fit();
            }

            // Remove the cleared chunks
            chunks_.resize(1);

            chunk_size_ = total_size;
        }
    }

private:
    size_t chunk_size_;
    std::vector<std::vector<T>> chunks_;
};
