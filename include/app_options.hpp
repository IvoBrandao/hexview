#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hexview {

/**
 * @class AppOptions
 * @brief Handles application command-line options parsing and management.
 *
 * This class allows you to define available command-line options, parse user-provided options,
 * check for the presence of options, retrieve their values, and display usage information.
 */
class AppOptions {
public:
    /**
     * @brief Adds a new command-line option to the list of available options.
     * @param flag The option flag (e.g., "--help").
     * @param description A description of what the option does.
     * @param takes_value Whether this option expects a value
     */
    void add_option(std::string_view flag, std::string_view description, bool takes_value = false);

    /**
     * @brief Parses the user-provided command-line arguments.
     * @param argc Argument count from main().
     * @param argv Argument vector from main().
     */
    void parse_user_options(int argc, char* argv[]);

    /**
     * @brief Checks if a specific option was provided by the user.
     * @param flag The option flag to check.
     * @return true if the option was provided, false otherwise.
     */
    bool has_option(std::string_view flag) const;

    /**
     * @brief Retrieves the value of a given option.
     * @param flag The option flag to retrieve.
     * @param def The default value to return if the option is not set.
     * @return The value of the option, or the default value if not set.
     */
    std::string get(std::string_view flag, const std::string& def = std::string()) const;

    /**
     * @brief Gets the positional arguments (non-option arguments).
     * @return Vector of positional arguments.
     */
    const std::vector<std::string>& get_positional_args() const;

    /**
     * @brief Displays usage information for all available options.
     * @param program_name The name of the program for usage display.
     */
    void show_usage(const std::string& program_name) const;

private:
    /**
     * @brief Stores the available options and their descriptions.
     */
    std::unordered_map<std::string, std::string> available_options_;

    /**
     * @brief Stores which options take values.
     */
    std::unordered_map<std::string, bool> option_takes_value_;

    /**
     * @brief Stores the options and values provided by the user.
     */
    std::unordered_map<std::string, std::string> user_options_;

    /**
     * @brief Stores positional arguments (non-option arguments).
     */
    std::vector<std::string> positional_args_;

    /**
     * @brief Checks if a string starts with a given prefix.
     * @param str The string to check.
     * @param prefix The prefix to look for.
     * @return true if str starts with prefix, false otherwise.
     */
    static bool starts_with(std::string_view str, std::string_view prefix);

    /**
     * @brief Parses a vector of argument strings into user options.
     * @param args The vector of argument strings.
     */
    void parse_options(std::vector<std::string_view>& args);

    /**
     * @brief Splits an argument into option and value parts.
     * @param arg The argument string to split.
     * @return A pair containing the option and its value.
     */
    std::pair<std::string_view, std::string_view> split_option_value(std::string_view arg);
};

} // namespace hexview