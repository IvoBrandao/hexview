#pragma once

#include "options.hpp"
#include "formatter.hpp"
#include "color.hpp"
#include <memory>

namespace hexview {

/**
 * @brief Main hex dumper class that orchestrates the dumping process
 */
class HexDumper {
public:
    /**
     * @brief Construct a hex dumper
     * @param options Configuration options
     */
    explicit HexDumper(const Options& options);

    /**
     * @brief Run the hex dump process
     * @return Exit code (0 for success)
     */
    int run();

private:
    Options options_;
    std::unique_ptr<Color> color_;
    std::unique_ptr<Formatter> formatter_;

    /**
     * @brief Setup input stream (file or stdin)
     * @return 0 for success, error code otherwise
     */
    int setup_input();

    /**
     * @brief Process input and generate hex dump
     * @return 0 for success, error code otherwise
     */
    int process_input();
};

} // namespace hexview