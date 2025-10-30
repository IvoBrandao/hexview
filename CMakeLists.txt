# CMakeLists.txt
# Modern CMake to build the xxd_cpp program
#
# Usage:
#   mkdir -p build
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build --config Release -- -j
# To enable Address/Undefined sanitizers (gcc/clang):
#   cmake -S . -B build -DENABLE_SANITIZERS=ON
#
# This file is self-contained and contains no abbreviations.

cmake_minimum_required(VERSION 3.16)

project(hexview
    VERSION 2025.2
    DESCRIPTION "hex dump tool"
    LANGUAGES CXX)

# Use C++20 by default
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Allow the user to enable sanitizers (Address/Undefined) for debugging builds.
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer (gcc/clang only)" OFF)

# Produce compile_commands.json for editors
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files - all source files in the modular design
set(SRCS
    main.cpp
    source/options.cpp
    source/color.cpp
    source/utils.cpp
    source/formatter.cpp
    source/dumper.cpp
    source/app_options.cpp
    source/options_parser.cpp
)

add_executable(hexview
    ${SRCS}
)

# Include directories for header files
target_include_directories(hexview PRIVATE include)

# Provide a VERSION macro to the code
target_compile_definitions(hexview PRIVATE XXD_CPP_VERSION="${PROJECT_VERSION}")

# Compiler warning flags and conservative defaults
if (MSVC)
    # /W4 is a strict warning level; you can switch to /W3 if it's too noisy.
    target_compile_options(hexview PRIVATE /W4 /permissive-)
else()
    # Use common warning flags for gcc/clang
    target_compile_options(hexview PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wformat=2
    )
endif()

# If sanitizers are enabled, add flags (only for clang/gcc-like compilers)
if (ENABLE_SANITIZERS AND NOT MSVC)
    message(STATUS "Enabling AddressSanitizer and UndefinedBehaviorSanitizer")
    target_compile_options(hexview PRIVATE -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all)
    target_link_options(hexview PRIVATE -fsanitize=address -fsanitize=undefined)
    # It's recommended to build with RelWithDebInfo or Debug when using sanitizers
endif()

# Install rules
install(TARGETS hexview
    RUNTIME DESTINATION bin
)

# Provide a small configurable option to build as a static binary (user sets on the command line)
option(BUILD_STATIC "Try to build a static executable (may fail on some platforms)" OFF)
if (BUILD_STATIC)
    if (MSVC)
        message(WARNING "Static build option is not supported/handled for MSVC in this CMake file.")
    else()
        # On Unix-like systems set STATIC linking preference for the target
        set_target_properties(hexview PROPERTIES LINK_SEARCH_START_STATIC ON LINK_SEARCH_END_STATIC ON)
        message(STATUS "Requested static build: linker given preference for static libraries.")
    endif()
endif()

# Useful messages for the user after configure
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Source files: ${SRCS}")
message(STATUS "Sanitizers enabled: ${ENABLE_SANITIZERS}")
message(STATUS "To build: mkdir -p build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release -- -j")

# End of CMakeLists.txt
