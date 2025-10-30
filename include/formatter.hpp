#pragma once

#include "options.hpp"
#include "color.hpp"
#include <vector>
#include <string>
#include <cstdint>

namespace hexview {

/**
 * @brief Handles formatting of hex dump output
 */
class Formatter {
public:
    /**
     * @brief Construct a formatter
     * @param options Configuration options
     * @param color Color manager
     */
    Formatter(const Options& options, const Color& color);

    /**
     * @brief Format a line of hex data
     * @param bytes Bytes to format
     * @param line_offset Offset of the first byte in the line
     */
    void format_line(const std::vector<unsigned char>& bytes, std::uint64_t line_offset) const;

private:
    const Options& options_;
    const Color& color_;

    /**
     * @brief Create hex column string
     * @param bytes Bytes to format
     * @return Formatted hex column
     */
    std::string make_hex_column(const std::vector<unsigned char>& bytes) const;

    /**
     * @brief Create ASCII column string
     * @param bytes Bytes to format
     * @return Formatted ASCII column
     */
    std::string make_ascii_column(const std::vector<unsigned char>& bytes) const;

    /**
     * @brief Print offset for the line
     * @param line_offset Offset to print
     */
    void print_offset(std::uint64_t line_offset) const;

    /**
     * @brief Print hex column with coloring
     * @param bytes Bytes to print
     */
    void print_hex_column_colored(const std::vector<unsigned char>& bytes) const;

    /**
     * @brief Print ASCII column with coloring
     * @param bytes Bytes to print
     */
    void print_ascii_column_colored(const std::vector<unsigned char>& bytes) const;
};

} // namespace hexview