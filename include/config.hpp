#pragma once

#include <cstddef>

// Configuration constants and version information
namespace hexview {

constexpr char const* VERSION = "Hexview 1.0";
constexpr size_t DEFAULT_BYTES_PER_LINE = 16;
constexpr size_t DEFAULT_GROUP_SIZE = 1;
constexpr size_t DEFAULT_OFFSET_WIDTH = 8;

// Enhanced buffer configuration for better performance
constexpr size_t MIN_READ_BLOCK_SIZE = 4096;        // 4KB minimum
constexpr size_t MAX_READ_BLOCK_SIZE = 1048576;     // 1MB maximum
constexpr size_t DEFAULT_READ_BLOCK_SIZE = 65536;   // 64KB default

// Large file support thresholds
constexpr size_t LARGE_FILE_THRESHOLD = 2147483648ULL;  // 2GB
constexpr size_t HUGE_FILE_THRESHOLD = 107374182400ULL; // 100GB

// Calculate optimal buffer size based on bytes per line
constexpr size_t calculate_optimal_buffer_size(size_t bytes_per_line) {
    // Target ~256 lines worth of data, but within reasonable bounds
    size_t target = bytes_per_line * 256;
    if (target < MIN_READ_BLOCK_SIZE) return MIN_READ_BLOCK_SIZE;
    if (target > MAX_READ_BLOCK_SIZE) return MAX_READ_BLOCK_SIZE;
    return target;
}

} // namespace hexview