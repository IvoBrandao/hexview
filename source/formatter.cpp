#include "formatter.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace hexview {

Formatter::Formatter(const Options& options, const Color& color)
    : options_(options), color_(color) {}

void Formatter::format_line(const std::vector<unsigned char>& bytes, std::uint64_t line_offset) const {
    if (!options_.ascii_only && !options_.hex_only) {
        // Both columns enabled
        print_offset(line_offset);
        if (options_.swap_columns) {
            // ASCII first
            print_ascii_column_colored(bytes);
            std::cout << " ";
            print_hex_column_colored(bytes);
        } else {
            // Hex first
            print_hex_column_colored(bytes);
            std::cout << " ";
            print_ascii_column_colored(bytes);
        }
        std::cout << '\n';
    } else if (options_.hex_only) {
        // hex only: omit ASCII
        print_offset(line_offset);
        print_hex_column_colored(bytes);
        std::cout << '\n';
    } else { // ascii_only
        print_offset(line_offset);
        print_ascii_column_colored(bytes);
        std::cout << '\n';
    }
}

std::string Formatter::make_hex_column(const std::vector<unsigned char>& bytes) const {
    std::ostringstream oss;
    const std::size_t BPL = options_.bytes_per_line;
    const std::size_t group = std::max<std::size_t>(1, options_.group);

    for (std::size_t i = 0; i < BPL; ++i) {
        if (i < bytes.size()) {
            unsigned int b = static_cast<unsigned int>(bytes[i]);
            oss << to_hex_byte(b, options_.uppercase);
        } else {
            oss << "  ";
        }
        if (i != BPL - 1) {
            if ((i % group) == (group - 1)) oss << "  ";
            else oss << " ";
        }
    }
    return oss.str();
}

std::string Formatter::make_ascii_column(const std::vector<unsigned char>& bytes) const {
    std::ostringstream oss;
    const std::size_t BPL = options_.bytes_per_line;

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        unsigned char ch = bytes[i];
        std::string rep = escape_byte(ch, options_.show_escapes, options_.show_non_printable_as_dot);
        oss << rep;
    }

    // Pad with spaces equal to missing bytes to keep the column aligned
    if (bytes.size() < BPL) {
        std::size_t pad = BPL - bytes.size();
        for (std::size_t p = 0; p < pad; ++p) oss << ' ';
    }
    return oss.str();
}

void Formatter::print_offset(std::uint64_t line_offset) const {
    if (options_.hide_offset) return;

    if (options_.offset_format == Options::OffsetFormat::Hex) {
        std::string off = to_hex_uint(line_offset, options_.offset_width, options_.uppercase);
        std::cout << off << ": ";
    } else {
        // decimal format (no leading zeros)
        std::cout << std::dec << line_offset << ": ";
        // restore formatting for other uses
        std::cout << std::hex << std::nouppercase;
    }
}

void Formatter::print_hex_column_colored(const std::vector<unsigned char>& bytes) const {
    const std::size_t BPL = options_.bytes_per_line;
    const std::size_t group = std::max<std::size_t>(1, options_.group);

    for (std::size_t i = 0; i < BPL; ++i) {
        if (i < bytes.size()) {
            unsigned int b = static_cast<unsigned int>(bytes[i]);
            bool printable = is_printable_ascii(static_cast<unsigned char>(b));
            if (!printable) color_.set(Color::Code::BrightYellow);
            else color_.set(Color::Code::BrightGreen);
            std::cout << to_hex_byte(b, options_.uppercase);
            color_.reset();
        } else {
            std::cout << "  ";
        }
        if (i != BPL - 1) {
            if ((i % group) == (group - 1)) std::cout << "  ";
            else std::cout << " ";
        }
    }
}

void Formatter::print_ascii_column_colored(const std::vector<unsigned char>& bytes) const {
    const std::size_t BPL = options_.bytes_per_line;

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        unsigned char ch = bytes[i];
        if (is_printable_ascii(ch)) {
            color_.set(Color::Code::BrightGreen);
            std::cout << static_cast<char>(ch);
        } else {
            color_.set(Color::Code::BrightYellow);
            if (options_.show_escapes) {
                std::string rep = escape_byte(ch, true, false);
                std::cout << rep;
            } else {
                std::cout << (options_.show_non_printable_as_dot ? '.' : '?');
            }
        }
        color_.reset();
    }

    // pad missing bytes visually (count bytes, not characters)
    if (bytes.size() < BPL) {
        std::size_t pad = BPL - bytes.size();
        for (std::size_t p = 0; p < pad; ++p) std::cout << ' ';
    }
}

} // namespace hexview