#include "options.hpp"
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

void Options::validate() {
    if (ascii_only && hex_only) {
        throw std::invalid_argument("options --ascii-only and --hex-only are mutually exclusive");
    }

    if (show_escapes) {
        show_non_printable_as_dot = false;
    }

    if (filename.empty()) {
        // If stdin is not a TTY or not interactive, use stdin
        if (!stdout_is_tty() || !::isatty(FILENO_STDIN)) {
            filename = "-";
        }
    }

#if defined(_WIN32) || defined(_WIN64)
    if (color && stdout_is_tty()) {
        enable_virtual_terminal_processing();
    }
#endif
}

void print_help_and_exit(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options] <file>\n\n"
              << "If <file> is '-' read from stdin.\n\n"
              << "Options:\n"
              << "  -n, --bytes-per-line N      Bytes per line (default 16)\n"
              << "  -g, --group G               Grouping of bytes for spacing (default 1)\n"
              << "  -o, --offset-width W        Offset width in hex digits when using hex offsets (default 8)\n"
              << "  -s, --start OFFSET          Start offset (decimal or 0x hex) (default 0)\n"
              << "  -l, --length LENGTH         Maximum number of bytes to read (0 = no limit)\n"
              << "  -u, --uppercase             Use uppercase hex letters\n"
              << "  -c, --color on|off|auto     Colorize output (auto = only when stdout is a TTY)\n"
              << "  --no-color                  Same as -c off\n"
              << "  -A, --ascii-only            Show ASCII only (no hex column)\n"
              << "  -H, --hex-only              Show hex only (no ASCII column)\n"
              << "  -S, --swap-columns          Print ASCII column first, hex column second\n"
              << "  --offset-format hex|dec     Show offsets in hex (default) or decimal\n"
              << "  --no-offset                 Hide the offset/address column\n"
              << "  --show-escapes              Show control escapes (\\n, \\r, \\t) and \\xHH for others\n"
              << "  -h, --help                  Show this help and exit\n"
              << "  --version                   Print version and exit\n\n"
              << "Examples:\n"
              << "  " << program_name << " file.bin\n"
              << "  " << program_name << " -n 8 -g 2 -u -c off file.bin\n"
              << "  cat file.bin | " << program_name << " -\n";
    std::exit(0);
}

Options parse_arguments(int argc, char* argv[]) {
    Options opt;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];

        if (a == "-h" || a == "--help") {
            opt.show_help = true;
            print_help_and_exit(argv[0]);
        } else if (a == "--version") {
            opt.show_version = true;
            std::cout << VERSION << '\n';
            std::exit(0);
        } else if (a == "-n" || a == "--bytes-per-line") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value");
            int val = std::stoi(argv[++i]);
            if (val <= 0) throw std::invalid_argument("bytes-per-line must be positive");
            opt.bytes_per_line = static_cast<std::size_t>(val);
        } else if (a == "-g" || a == "--group") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value");
            int val = std::stoi(argv[++i]);
            if (val <= 0) throw std::invalid_argument("group must be positive");
            opt.group = static_cast<std::size_t>(val);
        } else if (a == "-o" || a == "--offset-width") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value");
            int val = std::stoi(argv[++i]);
            if (val <= 0) throw std::invalid_argument("offset width must be positive");
            opt.offset_width = static_cast<std::size_t>(val);
        } else if (a == "-s" || a == "--start") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value");
            opt.start = parse_uint64(argv[++i]);
        } else if (a == "-l" || a == "--length") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value");
            opt.length = parse_uint64(argv[++i]);
        } else if (a == "-u" || a == "--uppercase") {
            opt.uppercase = true;
        } else if (a == "-c" || a == "--color") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value: on|off|auto");
            std::string v = argv[++i];
            std::transform(v.begin(), v.end(), v.begin(),
                          [](unsigned char ch){ return static_cast<char>(std::tolower(ch)); });
            if (v == "on") opt.color = true;
            else if (v == "off") opt.color = false;
            else if (v == "auto") opt.color = stdout_is_tty();
            else throw std::invalid_argument("invalid color option: " + v);
        } else if (a == "--no-color") {
            opt.color = false;
        } else if (a == "-A" || a == "--ascii-only") {
            opt.ascii_only = true;
        } else if (a == "-H" || a == "--hex-only") {
            opt.hex_only = true;
        } else if (a == "-S" || a == "--swap-columns") {
            opt.swap_columns = true;
        } else if (a == "--offset-format") {
            if (i + 1 >= argc) throw std::invalid_argument(a + " requires a value: hex|dec");
            std::string v = argv[++i];
            std::transform(v.begin(), v.end(), v.begin(),
                          [](unsigned char ch){ return static_cast<char>(std::tolower(ch)); });
            if (v == "hex") opt.offset_format = Options::OffsetFormat::Hex;
            else if (v == "dec") opt.offset_format = Options::OffsetFormat::Dec;
            else throw std::invalid_argument("invalid offset-format: " + v);
        } else if (a == "--no-offset") {
            opt.hide_offset = true;
        } else if (a == "--show-escapes") {
            opt.show_escapes = true;
            opt.show_non_printable_as_dot = false;
        } else if (!a.empty() && a[0] == '-') {
            throw std::invalid_argument("unknown option: " + a);
        } else {
            opt.filename = a;
        }
    }

    opt.validate();
    return opt;
}

} // namespace hexview