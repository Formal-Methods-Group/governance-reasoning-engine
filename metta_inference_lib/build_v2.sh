#!/bin/bash

# Build script for MeTTa Inference Library V2 improvements

echo "======================================"
echo "Building MeTTa Inference Library V2"
echo "======================================"

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_CLI=ON \
    -DBUILD_API=ON \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=ON \
    -DENABLE_LTO=ON

# Build
echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo ""
    echo "======================================"
    echo "Build successful!"
    echo "======================================"
    echo ""
    echo "Running V2 improvements test..."
    echo ""
    
    # Create config directory if needed
    if [ ! -d "../config" ]; then
        mkdir ../config
    fi
    
    # Run the test
    if [ -f "examples/test_v2_improvements" ]; then
        ./examples/test_v2_improvements
    else
        echo "Test executable not found. Please check the build output."
    fi
else
    echo ""
    echo "======================================"
    echo "Build failed! Check the errors above."
    echo "======================================"
    exit 1
fi