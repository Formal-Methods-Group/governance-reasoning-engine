#!/bin/bash

set -e

echo "Building MeTTa Inference Library..."

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_CLI=ON \
    -DBUILD_API=ON \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=ON

# Build
echo "Building..."
make -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 2)

echo "Build complete!"
echo ""
echo "To run tests: cd build && make test"
echo "To run CLI: ./build/metta_cli --help"
echo "To install: cd build && sudo make install"