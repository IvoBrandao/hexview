#pragma once

#include <iostream>

namespace hexview {

/**
 * @brief Manages terminal color output
 */
class Color {
public:
    enum class Code {
        Reset,
        BrightYellow,
        BrightGreen,
        BrightWhite,
    };

    /**
     * @brief Construct a Color manager
     * @param enabled Whether color output is enabled
     */
    explicit Color(bool enabled);

    /**
     * @brief Set color to the specified code
     * @param code Color code to set
     */
    void set(Code code) const;

    /**
     * @brief Reset color to default
     */
    void reset() const;

private:
    bool enabled_;
};

/**
 * @brief Check if stdout is connected to a terminal
 * @return true if stdout is a TTY
 */
bool stdout_is_tty();

/**
 * @brief Enable virtual terminal processing on Windows
 * @return true if successful (only on Windows)
 */
bool enable_virtual_terminal_processing();

/**
 * @brief Enhanced terminal color capability detection
 * @return true if terminal supports colors
 */
bool terminal_supports_color();

/**
 * @brief Get color support level of terminal
 * @return 0=none, 8=basic, 256=extended, 16777216=truecolor
 */
int get_color_support_level();

} // namespace hexview