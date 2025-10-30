# 🔍 Hexview - A Hex Dump Tool

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey.svg)](https://github.com/your-repo/hexview)

A hex dump utility written in C++20 that provides enhanced functionality over traditional tools like `xxd`. Hexview offers flexible output formatting, color support, and advanced display options for binary file analysis.

## ✨ Features

### 🎨 **Display Options**

- **Color Support**: Automatic terminal detection with customizable color output
- **Column Swapping**: Display ASCII first, then hex (`-S`/`--swap-columns`)
- **Flexible Formatting**: Customize bytes per line, grouping, and offset display
- **ASCII/Hex Only**: Show only ASCII (`-A`) or only hex (`-H`) columns
- **Escape Sequences**: Display control characters as escape sequences (`--show-escapes`)

### 📊 **Output Customization**

- **Offset Formats**: Choose between hexadecimal (default) or decimal offsets
- **Hide Offsets**: Remove address column entirely (`--no-offset`)
- **Uppercase Hex**: Use uppercase letters for hex digits (`-u`/`--uppercase`)
- **Custom Grouping**: Group bytes with customizable spacing
- **Line Length**: Adjust number of bytes per line (default: 16)

### 🎯 **Advanced Features**

- **Range Selection**: Start from specific offset and limit read length
- **Stdin Support**: Read from pipes or standard input
- **Cross-Platform**: Works on Windows, macOS, and Linux
- **Performance**: Optimized C++20 implementation with smart buffering

## 🚀 Quick Install

```bash
# macOS (with Homebrew) - if available
brew install hexview

# Ubuntu/Debian
sudo apt install build-essential cmake git
git clone https://github.com/your-username/hexview.git
cd hexview && mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build && sudo cmake --install build

# Or build locally and add to PATH
cmake --install build --prefix ~/.local
```

**➡️ [See full installation guide below](#installation) for all platforms**

## 🚀 Quick Start

### Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 11+, MSVC 2019+)
- CMake 3.16 or later

### Building

```bash
# Clone the repository
git clone https://github.com/your-username/hexview.git
cd hexview

# Build with CMake
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Installation

#### 🍎 **macOS**

##### Option 1: Build from Source (Recommended)

```bash
# Install Xcode Command Line Tools if not already installed
xcode-select --install

# Clone and build
git clone https://github.com/your-username/hexview.git
cd hexview
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Install to /usr/local/bin
sudo cmake --install build
```

##### Option 2: Using Homebrew (if available)

```bash
# If you have a Homebrew formula
brew install hexview

# Or install from local build
cp build/hexview /usr/local/bin/
```

#### 🐧 **Linux**

##### Ubuntu/Debian

```bash
# Install build dependencies
sudo apt update
sudo apt install build-essential cmake git

# Clone and build
git clone https://github.com/your-username/hexview.git
cd hexview
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Install system-wide
sudo cmake --install build

# Or install to user directory
cmake --install build --prefix ~/.local
# Add ~/.local/bin to PATH if not already there
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
```

##### CentOS/RHEL/Fedora

```bash
# Install build dependencies
sudo dnf install gcc-c++ cmake git  # Fedora
# sudo yum install gcc-c++ cmake3 git  # CentOS/RHEL

# Clone and build (same as Ubuntu)
git clone https://github.com/your-username/hexview.git
cd hexview
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
sudo cmake --install build
```

##### Arch Linux

```bash
# Install build dependencies
sudo pacman -S base-devel cmake git

# Clone and build
git clone https://github.com/your-username/hexview.git
cd hexview
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
sudo cmake --install build
```

#### 🪟 **Windows**

##### Option 1: Visual Studio (Recommended)

```cmd
# Install Visual Studio 2019 or later with C++ development tools
# Install Git for Windows and CMake

# Clone repository
git clone https://github.com/your-username/hexview.git
cd hexview

# Build with Visual Studio
mkdir build
cmake -S . -B build -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release

# Install to Program Files (run as Administrator)
cmake --install build

# Or copy to a directory in your PATH
copy build\Release\hexview.exe C:\Windows\System32\
```

##### Option 2: MinGW-w64/MSYS2

```bash
# In MSYS2 terminal
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake git

# Clone and build
git clone https://github.com/your-username/hexview.git
cd hexview
mkdir build
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Install
cmake --install build --prefix /mingw64
```

##### Option 3: Windows Subsystem for Linux (WSL)

```bash
# Follow the Linux instructions within WSL
# The binary will work in WSL environment
```

#### ✅ **Verify Installation**

After installation, verify it works:

```bash
# Check version
hexview --version

# Test with a simple file
echo "Hello, World!" | hexview -

# Expected output should show hex dump with colors (if terminal supports it)
```

#### 🔧 **Troubleshooting Installation**

**Missing CMake:**

- macOS: `brew install cmake` or download from cmake.org
- Linux: Use your package manager (`apt install cmake`, `dnf install cmake`, etc.)
- Windows: Download from cmake.org or use `winget install cmake`

**Compiler Issues:**

- Ensure you have a C++20 compatible compiler
- GCC 10+, Clang 11+, or MSVC 2019+ required

**Permission Issues:**

```bash
# On Unix systems, if you can't install system-wide:
cmake --install build --prefix ~/.local
export PATH="$HOME/.local/bin:$PATH"
```

## 📖 Usage

### Basic Usage

```bash
# View a file
./hexview file.bin

# Read from stdin
cat file.bin | ./hexview -

# View specific range
./hexview -s 0x100 -l 256 file.bin
```

### Advanced Examples

```bash
# ASCII column first, uppercase hex, no colors
./hexview -S -u -c off file.bin

# Compact view: 8 bytes per line, grouped by 2
./hexview -n 8 -g 2 file.bin

# ASCII only with escape sequences
./hexview -A --show-escapes file.bin

# Hex only without offsets
./hexview -H --no-offset file.bin

# Decimal offsets instead of hex
./hexview --offset-format dec file.bin
```

## 🎛️ Command Line Options

| Flag | Long Form | Description |
|------|-----------|-------------|
| `-h` | `--help` | Show help message and exit |
| | `--version` | Print version information |
| `-n N` | `--bytes-per-line N` | Bytes per line (default: 16) |
| `-g G` | `--group G` | Byte grouping for spacing (default: 1) |
| `-o W` | `--offset-width W` | Offset width in hex digits (default: 8) |
| `-s OFFSET` | `--start OFFSET` | Start offset (decimal or 0x hex) |
| `-l LENGTH` | `--length LENGTH` | Maximum bytes to read (0 = unlimited) |
| `-u` | `--uppercase` | Use uppercase hex letters |
| `-c MODE` | `--color MODE` | Color mode: `on`\|`off`\|`auto` |
| | `--no-color` | Disable color output |
| `-A` | `--ascii-only` | Show ASCII column only |
| `-H` | `--hex-only` | Show hex column only |
| `-S` | `--swap-columns` | ASCII first, then hex |
| | `--offset-format FORMAT` | Offset format: `hex`\|`dec` |
| | `--no-offset` | Hide offset/address column |
| | `--show-escapes` | Show control character escapes |

## 🏗️ Architecture

Hexview is built with a modern, modular C++ architecture:

```text
📁 Project Structure
├── 📄 main.cpp              # Application entry point
├── 📁 include/              # Header files
│   ├── 📄 config.hpp        # Configuration constants
│   ├── 📄 options.hpp       # Option structures
│   ├── 📄 color.hpp         # Color management
│   ├── 📄 utils.hpp         # Utility functions
│   ├── 📄 formatter.hpp     # Output formatting
│   ├── 📄 dumper.hpp        # Main dumper class
│   ├── 📄 app_options.hpp   # CLI argument parser
│   └── 📄 options_parser.hpp # Options integration
└── 📁 source/               # Implementation files
    ├── 📄 options.cpp
    ├── 📄 color.cpp
    ├── 📄 utils.cpp
    ├── 📄 formatter.cpp
    ├── 📄 dumper.cpp
    ├── 📄 app_options.cpp
    └── 📄 options_parser.cpp
```

### Key Components

- **🎨 Color System**: Cross-platform terminal color support with automatic detection
- **⚙️ Options Parser**: Sophisticated CLI argument parsing with validation
- **📝 Formatter**: Flexible output formatting engine
- **🔄 Dumper**: Main processing engine with optimized I/O
- **🛠️ Utils**: Common utilities for parsing and character handling

## 🚀 Performance & Optimizations

### Large File Handling

- **64-bit File Support**: Uses `std::uint64_t` for offsets and sizes, supporting files up to ~18 exabytes
- **Streaming Architecture**: Memory-efficient processing that doesn't load entire files into memory
- **Adaptive Buffering**: Intelligent buffer sizing based on file characteristics and system capabilities

### Buffer Optimization

```cpp
// Adaptive buffer sizing based on bytes per line
constexpr size_t MIN_BUFFER_SIZE = 4096;
constexpr size_t MAX_BUFFER_SIZE = 1048576; // 1MB

size_t optimal_buffer = std::max(MIN_BUFFER_SIZE,
    bytes_per_line * 256); // 256 lines worth
optimal_buffer = std::min(optimal_buffer, MAX_BUFFER_SIZE);
```

### Terminal Detection

- **Enhanced Color Detection**: Checks `COLORTERM`, `TERM` environment variables
- **Windows Console Support**: Automatic Virtual Terminal Processing enablement
- **Fallback Modes**: Graceful degradation for unsupported terminals

## 📋 Examples

### Standard Output

```sh
00000000: 48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a 54 68 69 Hello World!.Thi
00000010: 73 20 69 73 20 61 20 74 65 73 74 2e              s is a test.
```

### Swapped Columns (`-S`)

```sh
00000000: Hello World!.Thi 48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a 54 68 69
00000010: s is a test.     73 20 69 73 20 61 20 74 65 73 74 2e
```

### ASCII Only (`-A`)

```sh
00000000: Hello World!.Thi
00000010: s is a test.
```

### Hex Only (`-H`)

```sh
00000000: 48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a 54 68 69
00000010: 73 20 69 73 20 61 20 74 65 73 74 2e
```

## 🔧 Development

### Building with Debug Info

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Enable Sanitizers (GCC/Clang)

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON
cmake --build build
```

### Optimized Release Build

```bash
# Maximum optimization for performance
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native"
cmake --build build
```

### Testing Large Files

```bash
# Generate a large test file (1GB)
dd if=/dev/urandom of=large_test.bin bs=1M count=1024

# Test with different buffer strategies
./build/hexview -n 32 large_test.bin | head -20
```

### Running Tests

```bash
# Create test file
echo "Hello, World!" > test.txt

# Test various features
./build/hexview test.txt
./build/hexview -S -u test.txt
./build/hexview --ascii-only test.txt
```

## 🔧 Troubleshooting

### Color Issues

If colors aren't working in your terminal:

```bash
# Force color output
./hexview --color on file.bin

# Check terminal capabilities
echo $TERM
echo $COLORTERM

# Test basic ANSI color support
printf "\x1b[31mRed\x1b[0m text\n"
```

### Large File Performance

For files over 2GB:

```bash
# Use larger buffer for better performance
./hexview -n 32 huge_file.bin

# Limit output for quick preview
./hexview -l 1024 huge_file.bin
```

### Memory Usage

Monitor memory usage with large files:

```bash
# Check memory usage during operation
/usr/bin/time -v ./hexview large_file.bin > /dev/null
```

### Current Limitations

- Very large files (>100GB) may require additional memory optimization
- Some legacy terminal emulators may not support all color features
- Memory usage scales with bytes-per-line setting for very wide displays

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by the classic `xxd` utility
- Built with modern C++20 features
