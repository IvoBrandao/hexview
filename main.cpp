#include "dumper.hpp"
#include "options_parser.hpp"
#include <iostream>
#include <stdexcept>


int main(int argc, char* argv[]) {
    try {
        hexview::OptionsParser parser;
        hexview::Options options = parser.parse(argc, argv);
        hexview::HexDumper dumper(options);
        return dumper.run();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << "Use --help to show usage.\n";
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << "\n";
        return 3;
    }
}
