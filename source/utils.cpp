#include "utils.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace hexview {

std::uint64_t parse_uint64(const std::string& s) {
    std::uint64_t val = 0;
    try {
        size_t idx = 0;
        int base = 10;
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            base = 16;
        }
        val = std::stoull(s, &idx, base);
        (void)idx;
    } catch (const std::exception&) {
        throw std::invalid_argument("invalid numeric value: " + s);
    }
    return val;
}

std::string to_hex_uint(std::uint64_t value, std::size_t width, bool uppercase) {
    std::ostringstream oss;
    oss << std::hex << std::setw(static_cast<int>(width)) << std::setfill('0')
        << (uppercase ? std::uppercase : std::nouppercase) << value;
    return oss.str();
}

std::string to_hex_byte(unsigned int b, bool uppercase) {
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0')
        << (uppercase ? std::uppercase : std::nouppercase) << (b & 0xFFu);
    return oss.str();
}

bool is_printable_ascii(unsigned char ch) {
    return (ch >= 32 && ch <= 126);
}

std::string escape_byte(unsigned char ch, bool show_escapes, bool ascii_dot_if_not) {
    if (is_printable_ascii(ch)) {
        return std::string(1, static_cast<char>(ch));
    }

    if (!show_escapes) {
        return ascii_dot_if_not ? std::string(1, '.') : std::string(1, '?');
    }

    // show escapes for common controls, otherwise \xHH
    switch (ch) {
        case '\n': return std::string("\\n");
        case '\r': return std::string("\\r");
        case '\t': return std::string("\\t");
        default: {
            std::ostringstream oss;
            oss << "\\x" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(ch & 0xFF);
            return oss.str();
        }
    }
}

} // namespace hexview