#pragma once

#include <cstdint>
#include <string>

namespace hexview {

/**
 * @brief Parse a uint64 value from string (supports hex with 0x prefix)
 * @param s String to parse
 * @return Parsed value
 * @throws std::invalid_argument if string is invalid
 */
std::uint64_t parse_uint64(const std::string& s);

/**
 * @brief Convert value to hex string with specified width
 * @param value Value to convert
 * @param width Minimum width (with leading zeros)
 * @param uppercase Whether to use uppercase letters
 * @return Hex string representation
 */
std::string to_hex_uint(std::uint64_t value, std::size_t width, bool uppercase);

/**
 * @brief Convert byte to 2-character hex string
 * @param b Byte value
 * @param uppercase Whether to use uppercase letters
 * @return Hex string representation
 */
std::string to_hex_byte(unsigned int b, bool uppercase);

/**
 * @brief Check if character is printable ASCII
 * @param ch Character to check
 * @return true if printable ASCII (32-126)
 */
bool is_printable_ascii(unsigned char ch);

/**
 * @brief Convert byte to escaped string representation
 * @param ch Character to escape
 * @param show_escapes Whether to show escape sequences
 * @param ascii_dot_if_not Whether to use '.' for non-printable if not showing escapes
 * @return String representation of the character
 */
std::string escape_byte(unsigned char ch, bool show_escapes, bool ascii_dot_if_not);

} // namespace hexview