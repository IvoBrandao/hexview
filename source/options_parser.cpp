#include "options_parser.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "color.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
#  include <io.h>
#  define ISATTY _isatty
#  define FILENO_STDIN _fileno(stdin)
#else
#  include <unistd.h>
#  define ISATTY isatty
#  define FILENO_STDIN fileno(stdin)
#endif

namespace hexview {

OptionsParser::OptionsParser() {
    setup_options();
}

void OptionsParser::setup_options() {
    // Boolean flags (no value)
    app_options_.add_option("-h", "Show this help and exit", false);
    app_options_.add_option("--help", "Show this help and exit", false);
    app_options_.add_option("--version", "Print version and exit", false);
    app_options_.add_option("-u", "Use uppercase hex letters", false);
    app_options_.add_option("--uppercase", "Use uppercase hex letters", false);
    app_options_.add_option("--no-color", "Same as -c off", false);
    app_options_.add_option("-A", "Show ASCII only (no hex column)", false);
    app_options_.add_option("--ascii-only", "Show ASCII only (no hex column)", false);
    app_options_.add_option("-H", "Show hex only (no ASCII column)", false);
    app_options_.add_option("--hex-only", "Show hex only (no ASCII column)", false);
    app_options_.add_option("-S", "Print ASCII column first, hex column second", false);
    app_options_.add_option("--swap-columns", "Print ASCII column first, hex column second", false);
    app_options_.add_option("--no-offset", "Hide the offset/address column", false);
    app_options_.add_option("--show-escapes", "Show control escapes (\\n, \\r, \\t) and \\xHH for others", false);

    // Options that take values
    app_options_.add_option("-n", "Bytes per line (default 16)", true);
    app_options_.add_option("--bytes-per-line", "Bytes per line (default 16)", true);
    app_options_.add_option("-g", "Grouping of bytes for spacing (default 1)", true);
    app_options_.add_option("--group", "Grouping of bytes for spacing (default 1)", true);
    app_options_.add_option("-o", "Offset width in hex digits when using hex offsets (default 8)", true);
    app_options_.add_option("--offset-width", "Offset width in hex digits when using hex offsets (default 8)", true);
    app_options_.add_option("-s", "Start offset (decimal or 0x hex) (default 0)", true);
    app_options_.add_option("--start", "Start offset (decimal or 0x hex) (default 0)", true);
    app_options_.add_option("-l", "Maximum number of bytes to read (0 = no limit)", true);
    app_options_.add_option("--length", "Maximum number of bytes to read (0 = no limit)", true);
    app_options_.add_option("-c", "Colorize output (on|off|auto - auto = only when stdout is a TTY)", true);
    app_options_.add_option("--color", "Colorize output (on|off|auto - auto = only when stdout is a TTY)", true);
    app_options_.add_option("--offset-format", "Show offsets in hex (default) or decimal", true);
}

Options OptionsParser::parse(int argc, char* argv[]) {
    app_options_.parse_user_options(argc, argv);
    return convert_to_options(argv[0]);
}

void OptionsParser::show_help_and_exit(const char* program_name) {
    app_options_.show_usage(program_name);
    std::exit(0);
}

Options OptionsParser::convert_to_options(const char* program_name) {
    Options opt;

    // Handle help and version first
    if (app_options_.has_option("-h") || app_options_.has_option("--help")) {
        opt.show_help = true;
        show_help_and_exit(program_name);
    }

    if (app_options_.has_option("--version")) {
        opt.show_version = true;
        std::cout << VERSION << '\n';
        std::exit(0);
    }

    // Parse numeric options
    if (app_options_.has_option("-n") || app_options_.has_option("--bytes-per-line")) {
        std::string val = app_options_.get("-n", app_options_.get("--bytes-per-line"));
        if (!val.empty()) {
            int parsed_val = std::stoi(val);
            if (parsed_val <= 0) throw std::invalid_argument("bytes-per-line must be positive");
            opt.bytes_per_line = static_cast<std::size_t>(parsed_val);
        }
    }

    if (app_options_.has_option("-g") || app_options_.has_option("--group")) {
        std::string val = app_options_.get("-g", app_options_.get("--group"));
        if (!val.empty()) {
            int parsed_val = std::stoi(val);
            if (parsed_val <= 0) throw std::invalid_argument("group must be positive");
            opt.group = static_cast<std::size_t>(parsed_val);
        }
    }

    if (app_options_.has_option("-o") || app_options_.has_option("--offset-width")) {
        std::string val = app_options_.get("-o", app_options_.get("--offset-width"));
        if (!val.empty()) {
            int parsed_val = std::stoi(val);
            if (parsed_val <= 0) throw std::invalid_argument("offset width must be positive");
            opt.offset_width = static_cast<std::size_t>(parsed_val);
        }
    }

    if (app_options_.has_option("-s") || app_options_.has_option("--start")) {
        std::string val = app_options_.get("-s", app_options_.get("--start"));
        if (!val.empty()) {
            opt.start = parse_uint64(val);
        }
    }

    if (app_options_.has_option("-l") || app_options_.has_option("--length")) {
        std::string val = app_options_.get("-l", app_options_.get("--length"));
        if (!val.empty()) {
            opt.length = parse_uint64(val);
        }
    }

    // Boolean flags
    if (app_options_.has_option("-u") || app_options_.has_option("--uppercase")) {
        opt.uppercase = true;
    }

    if (app_options_.has_option("-A") || app_options_.has_option("--ascii-only")) {
        opt.ascii_only = true;
    }

    if (app_options_.has_option("-H") || app_options_.has_option("--hex-only")) {
        opt.hex_only = true;
    }

    if (app_options_.has_option("-S") || app_options_.has_option("--swap-columns")) {
        opt.swap_columns = true;
    }

    if (app_options_.has_option("--no-offset")) {
        opt.hide_offset = true;
    }

    if (app_options_.has_option("--show-escapes")) {
        opt.show_escapes = true;
        opt.show_non_printable_as_dot = false;
    }

    // Color handling
    if (app_options_.has_option("--no-color")) {
        opt.color = false;
    } else if (app_options_.has_option("-c") || app_options_.has_option("--color")) {
        std::string val = app_options_.get("-c", app_options_.get("--color"));
        if (!val.empty()) {
            std::transform(val.begin(), val.end(), val.begin(),
                          [](unsigned char ch){ return static_cast<char>(std::tolower(ch)); });
            if (val == "on") opt.color = true;
            else if (val == "off") opt.color = false;
            else if (val == "auto") opt.color = stdout_is_tty();
            else throw std::invalid_argument("invalid color option: " + val);
        }
    }

    // Offset format
    if (app_options_.has_option("--offset-format")) {
        std::string val = app_options_.get("--offset-format");
        if (!val.empty()) {
            std::transform(val.begin(), val.end(), val.begin(),
                          [](unsigned char ch){ return static_cast<char>(std::tolower(ch)); });
            if (val == "hex") opt.offset_format = Options::OffsetFormat::Hex;
            else if (val == "dec") opt.offset_format = Options::OffsetFormat::Dec;
            else throw std::invalid_argument("invalid offset-format: " + val);
        }
    }

    // Handle filename from positional arguments
    const auto& positional = app_options_.get_positional_args();
    if (!positional.empty()) {
        opt.filename = positional[0];
    }

    opt.validate();
    return opt;
}

} // namespace hexview