#pragma once

#include <cstdint>
#include <string>

namespace hexview {

/**
 * @brief Configuration options for the hex viewer
 */
struct Options {
    enum class OffsetFormat { Hex, Dec };

    std::string filename = "";                       // "-" => stdin
    std::uint64_t start = 0;                        // start offset in bytes
    std::uint64_t length = 0;                       // 0 => no limit
    std::size_t bytes_per_line = 16;                // how many bytes per line
    std::size_t group = 1;                          // grouping of bytes for spacing
    std::size_t offset_width = 8;                   // width in hex digits for offset when hex shown
    bool uppercase = false;                         // uppercase hex digits
    bool color = true;                              // enable color output if terminal supports it
    bool ascii_only = false;                        // show only ASCII (no hex)
    bool hex_only = false;                          // show only hex (no ASCII)
    bool show_help = false;
    bool show_version = false;
    bool show_non_printable_as_dot = true;          // whether to replace non-printable with '.'
    bool swap_columns = false;                      // ASCII left, hex right
    bool hide_offset = false;                       // do not print offset column
    bool show_escapes = false;                      // show escapes for control chars and \xHH for others
    OffsetFormat offset_format = OffsetFormat::Hex;

    /**
     * @brief Validate options for conflicts and set defaults
     * @throws std::invalid_argument if options are invalid
     */
    void validate();
};

/**
 * @brief Parse command line arguments into Options
 * @param argc Argument count
 * @param argv Argument vector
 * @return Parsed options
 * @throws std::invalid_argument if arguments are invalid
 */
Options parse_arguments(int argc, char* argv[]);

/**
 * @brief Print help message and exit
 * @param program_name Name of the program
 */
void print_help_and_exit(const char* program_name);

} // namespace hexview