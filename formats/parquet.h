#pragma once

#include <string>

// Parquet file format specification:
// https://github.com/apache/parquet-format/tree/master

/**
 * The primitive types supported by Parquet.  These types are intended to be used in
 * combination with the encodings to control the on disk storage format.
 */
enum class PrimitiveType
{
    BOOLEAN,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    BYTE_ARRAY,
    FIXED_LEN_BYTE_ARRAY
};