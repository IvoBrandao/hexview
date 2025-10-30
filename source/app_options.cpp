#include "app_options.hpp"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace hexview {

void AppOptions::add_option(std::string_view flag, std::string_view description, bool takes_value) {
    available_options_[std::string(flag)] = std::string(description);
    option_takes_value_[std::string(flag)] = takes_value;
}

void AppOptions::show_usage(const std::string& program_name) const {
    std::cout << "Usage: " << program_name << " [options] <file>\n\n";
    std::cout << "If <file> is '-' read from stdin.\n\n";
    std::cout << "Options:\n";

    // Find the maximum flag width for alignment
    size_t max_flag_width = 0;
    for (const auto& [flag, desc] : available_options_) {
        max_flag_width = std::max(max_flag_width, flag.length());
    }

    // Add some padding
    max_flag_width += 2;

    for (const auto& [flag, desc] : available_options_) {
        std::cout << "  " << std::left << std::setw(static_cast<int>(max_flag_width)) << flag << " " << desc << "\n";
    }
    std::cout << "\nExamples:\n";
    std::cout << "  " << program_name << " file.bin\n";
    std::cout << "  " << program_name << " -n 8 -g 2 -u -c off file.bin\n";
    std::cout << "  cat file.bin | " << program_name << " -\n";
}

bool AppOptions::has_option(std::string_view flag) const {
    return user_options_.find(std::string(flag)) != user_options_.end();
}

std::string AppOptions::get(std::string_view flag, const std::string& def) const {
    auto key = std::string(flag);
    auto it = user_options_.find(key);
    if (it != user_options_.end())
        return it->second;
    return def;
}

const std::vector<std::string>& AppOptions::get_positional_args() const {
    return positional_args_;
}

void AppOptions::parse_user_options(int argc, char* argv[]) {
    std::vector<std::string_view> args(argv + 1, argv + argc);
    try {
        parse_options(args);
    } catch (const std::exception& e) {
        std::cerr << "Option parse error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

bool AppOptions::starts_with(std::string_view str, std::string_view prefix) {
    return str.substr(0, prefix.size()) == prefix;
}

std::pair<std::string_view, std::string_view> AppOptions::split_option_value(std::string_view arg) {
    auto pos = arg.find('=');
    if (pos == std::string_view::npos) {
        throw std::runtime_error("Invalid argument format: " + std::string(arg));
    }
    return {arg.substr(0, pos), arg.substr(pos + 1)};
}

void AppOptions::parse_options(std::vector<std::string_view>& args) {
    for (auto it = args.begin(); it != args.end(); ++it) {
        auto flag = *it;

        // Handle positional arguments (non-options)
        if (!starts_with(flag, "-")) {
            positional_args_.push_back(std::string(flag));
            continue;
        }

        if (auto eq = flag.find('='); eq != std::string_view::npos) {
            // --flag=value form
            auto [opt, val] = split_option_value(flag);
            if (!available_options_.count(std::string(opt)))
                throw std::runtime_error("Invalid option: " + std::string(opt));
            user_options_[std::string(opt)] = std::string(val);
        } else {
            // --flag value or standalone --flag
            auto key = std::string(flag);
            if (!available_options_.count(key))
                throw std::runtime_error("Invalid option: " + key);

            // Check if this option takes a value
            bool takes_value = option_takes_value_[key];

            if (takes_value) {
                auto next = std::next(it);
                if (next != args.end() && !starts_with(*next, "-")) {
                    user_options_[key] = std::string(*next);
                    ++it; // skip the value
                } else {
                    throw std::runtime_error("Option " + key + " requires a value");
                }
            } else {
                user_options_[key] = "";
            }
        }
    }
}

} // namespace hexview