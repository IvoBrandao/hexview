#pragma once

#include "options.hpp"
#include "app_options.hpp"

namespace hexview {

/**
 * @brief Enhanced options parser that integrates AppOptions with the existing Options system
 */
class OptionsParser {
public:
    /**
     * @brief Construct the parser and setup available options
     */
    OptionsParser();

    /**
     * @brief Parse arguments using the enhanced CLI parser
     * @param argc Argument count
     * @param argv Argument vector
     * @return Parsed Options structure
     * @throws std::invalid_argument if parsing fails
     */
    Options parse(int argc, char* argv[]);

    /**
     * @brief Show help and exit
     * @param program_name Program name for usage display
     */
    void show_help_and_exit(const char* program_name);

private:
    AppOptions app_options_;

    /**
     * @brief Convert AppOptions results to Options structure
     * @param program_name Program name for error reporting
     * @return Populated Options structure
     */
    Options convert_to_options(const char* program_name);

    /**
     * @brief Setup all available command line options
     */
    void setup_options();
};

} // namespace hexview