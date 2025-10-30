#include "color.hpp"
#include <cstdlib>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#  include <io.h>
#  include <windows.h>
#  include <fcntl.h>
#  define ISATTY _isatty
#  define FILENO_STDOUT _fileno(stdout)
#else
#  include <unistd.h>
#  define ISATTY isatty
#  define FILENO_STDOUT fileno(stdout)
#endif

namespace hexview {

Color::Color(bool enabled) : enabled_(enabled) {}

void Color::set(Code code) const {
    if (!enabled_) return;

    switch (code) {
        case Code::BrightYellow: std::cout << "\x1b[1;33m"; break;
        case Code::BrightGreen:  std::cout << "\x1b[1;32m"; break;
        case Code::BrightWhite:  std::cout << "\x1b[1;37m"; break;
        case Code::Reset:        std::cout << "\x1b[0m";    break;
    }
}

void Color::reset() const {
    set(Code::Reset);
}

bool stdout_is_tty() {
#if defined(_WIN32) || defined(_WIN64)
    return _isatty(_fileno(stdout)) != 0;
#else
    return isatty(fileno(stdout)) != 0;
#endif
}

bool enable_virtual_terminal_processing() {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
    return SetConsoleMode(hOut, dwMode) != 0;
#else
    return true; // Not needed on Unix-like systems
#endif
}

bool terminal_supports_color() {
    // First check if we're connected to a terminal
    if (!stdout_is_tty()) {
        return false;
    }

    // Check environment variables that indicate color support
    const char* colorterm = std::getenv("COLORTERM");
    if (colorterm && (std::strcmp(colorterm, "truecolor") == 0 ||
                     std::strcmp(colorterm, "24bit") == 0)) {
        return true;
    }

    const char* term = std::getenv("TERM");
    if (term) {
        // Common terminals that support color
        if (std::strstr(term, "color") != nullptr ||
            std::strstr(term, "xterm") != nullptr ||
            std::strstr(term, "screen") != nullptr ||
            std::strstr(term, "tmux") != nullptr ||
            std::strcmp(term, "linux") == 0) {
            return true;
        }
    }

    // Check for common CI environments that support color
    if (std::getenv("CI") || std::getenv("GITHUB_ACTIONS") ||
        std::getenv("TRAVIS") || std::getenv("CIRCLECI")) {
        return true;
    }

#if defined(_WIN32) || defined(_WIN64)
    // Try to enable VT processing on Windows
    return enable_virtual_terminal_processing();
#endif

    return false;
}

int get_color_support_level() {
    if (!terminal_supports_color()) {
        return 0;
    }

    const char* colorterm = std::getenv("COLORTERM");
    if (colorterm && (std::strcmp(colorterm, "truecolor") == 0 ||
                     std::strcmp(colorterm, "24bit") == 0)) {
        return 16777216; // 24-bit truecolor
    }

    const char* term = std::getenv("TERM");
    if (term && std::strstr(term, "256color") != nullptr) {
        return 256;
    }

    return 8; // Basic 8-color support
}

} // namespace hexview