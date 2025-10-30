#include "dumper.hpp"
#include "config.hpp"
#include "color.hpp"
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64)
#  include <io.h>
#  include <fcntl.h>
#endif

namespace hexview {

HexDumper::HexDumper(const Options& options) : options_(options) {
    bool has_color_support = terminal_supports_color();
    color_ = std::make_unique<Color>(options_.color && has_color_support);
    formatter_ = std::make_unique<Formatter>(options_, *color_);
}

int HexDumper::run() {
    return process_input();
}

int HexDumper::process_input() {
    std::istream* in_ptr = nullptr;
    std::ifstream file_stream;

    if (options_.filename == "-") {
#if defined(_WIN32) || defined(_WIN64)
        _setmode(_fileno(stdin), _O_BINARY);
#endif
        in_ptr = &std::cin;
        std::cin.sync_with_stdio(false);
        std::cin.tie(nullptr);
    } else {
        file_stream.open(options_.filename, std::ios::binary);
        if (!file_stream.is_open()) {
            std::cerr << "Error: failed to open file '" << options_.filename << "'\n";
            return 1;
        }
        in_ptr = &file_stream;
    }

    std::istream& in = *in_ptr;

    // Seek to start if file is seekable and a file (not "-")
    if (options_.filename != "-") {
        if (options_.start != 0) {
            file_stream.clear();
            file_stream.seekg(static_cast<std::streamoff>(options_.start), std::ios::beg);
            if (!file_stream.good()) {
                std::cerr << "Error: seeking to start offset " << options_.start << " failed.\n";
                return 2;
            }
        }
    } else {
        if (options_.start != 0) {
            std::uint64_t to_skip = options_.start;
            std::array<char, 4096> skipbuf;
            while (to_skip > 0 && in.good()) {
                std::streamsize chunk = static_cast<std::streamsize>(
                    std::min<std::uint64_t>(skipbuf.size(), to_skip));
                in.read(skipbuf.data(), chunk);
                std::streamsize got = in.gcount();
                if (got <= 0) break;
                to_skip -= static_cast<std::uint64_t>(got);
            }
            if (to_skip != 0) {
                std::cerr << "Warning: could not skip to start offset; input too short.\n";
            }
        }
    }

    const std::size_t BPL = options_.bytes_per_line;
    const std::size_t read_block = calculate_optimal_buffer_size(BPL);

    std::vector<unsigned char> line_buf;
    line_buf.reserve(BPL);

    std::vector<unsigned char> buffer(read_block);
    std::uint64_t offset = options_.start;
    std::uint64_t remaining = options_.length; // 0 => unlimited
    bool eof_reached = false;

    while (!eof_reached) {
        std::streamsize want = static_cast<std::streamsize>(read_block);
        if (remaining != 0) {
            if (remaining < static_cast<std::uint64_t>(want)) {
                want = static_cast<std::streamsize>(remaining);
            }
        }

        in.read(reinterpret_cast<char*>(buffer.data()), want);
        std::streamsize got = in.gcount();
        if (got <= 0) {
            eof_reached = true;
            break;
        }

        for (std::streamsize i = 0; i < got; ++i) {
            unsigned char b = buffer[static_cast<std::size_t>(i)];
            line_buf.push_back(b);

            if (line_buf.size() == BPL) {
                std::uint64_t first_byte_offset = offset - static_cast<std::uint64_t>(line_buf.size());
                formatter_->format_line(line_buf, first_byte_offset);
                line_buf.clear();
            }

            offset++;
            if (remaining != 0) {
                if (remaining > 0) {
                    remaining--;
                    if (remaining == 0) {
                        eof_reached = true;
                        break;
                    }
                }
            }
        }
    }

    if (!line_buf.empty()) {
        std::uint64_t first_byte_offset = offset - static_cast<std::uint64_t>(line_buf.size());
        formatter_->format_line(line_buf, first_byte_offset);
    }

    return 0;
}

} // namespace hexview