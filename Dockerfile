# Multi-stage Dockerfile for Governance Reasoning Engine
# Stage 1: Build C++ components
FROM ubuntu:22.04 AS cpp-builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Copy C++ source files
WORKDIR /build
COPY metta_inference_lib/ ./metta_inference_lib/

# Build metta_inference_lib
WORKDIR /build/metta_inference_lib
RUN mkdir -p build && \
    cd build && \
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_CLI=ON \
        -DBUILD_API=ON \
        -DBUILD_TESTS=OFF \
        -DBUILD_EXAMPLES=OFF && \
    make -j$(nproc)

# Stage 2: Build Rust components
FROM rust:latest AS rust-builder

# Install additional dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

# Copy Rust source files
WORKDIR /build
COPY hyperon-experimental-main/ ./hyperon-experimental-main/

# Build Rust components
WORKDIR /build/hyperon-experimental-main
RUN cargo build --release

# Build the C bindings for hyperon
WORKDIR /build/hyperon-experimental-main/c
RUN cargo build --release

# Stage 3: Python setup and final image
FROM python:3.11-slim

# Install runtime and build dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libgomp1 \
    git \
    gcc \
    g++ \
    cmake \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install Python build tools
RUN pip install --no-cache-dir --upgrade pip setuptools wheel conan

# Create app directory
WORKDIR /app

# Copy built C++ binaries from cpp-builder
COPY --from=cpp-builder /build/metta_inference_lib/build/metta_cli /usr/local/bin/
COPY --from=cpp-builder /build/metta_inference_lib/build/metta_knowledge_cli /usr/local/bin/
COPY --from=cpp-builder /build/metta_inference_lib/build/*.a /usr/local/lib/

# Copy built Rust binaries from rust-builder
COPY --from=rust-builder /build/hyperon-experimental-main/target/release/metta-repl /usr/local/bin/
COPY --from=rust-builder /build/hyperon-experimental-main/target/release/libhyperonc.a /usr/local/lib/

# Set environment for hyperonc location
ENV HYPERONC_PATH=/usr/local

# Copy Python package source - skip installation for now due to missing dependencies
# The Python bindings would require all C build artifacts which are complex to setup
COPY hyperon-experimental-main/python/ /app/hyperon-python/

# Note: Python hyperon module installation is skipped as it requires complex C++ bindings
# Users can still use the metta-repl and metta_cli commands directly

# Copy all Metta files and knowledge bases
WORKDIR /app
COPY base/ ./base/
COPY example/ ./example/
COPY knowledge/ ./knowledge/
COPY reason/ ./reason/
COPY hyperon-experimental-main/lib/src/metta/runner/ ./runner/
COPY metta_inference_lib/config/ ./config/

# Set up library path
ENV LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}
ENV PYTHONPATH=/app/hyperon-python:${PYTHONPATH}

# Create a working directory for user files
RUN mkdir -p /workspace
WORKDIR /workspace

# Create a simple test script to verify installation
RUN echo '#!/usr/bin/env bash\n\
echo "Testing Governance Reasoning Engine components..."\n\
echo ""\n\
echo "1. Testing MeTTa CLI..."\n\
if /usr/local/bin/metta_cli --help > /dev/null 2>&1; then\n\
    echo "   ✓ MeTTa CLI is working"\n\
else\n\
    echo "   ✗ MeTTa CLI failed"\n\
fi\n\
echo ""\n\
echo "2. Testing MeTTa Knowledge CLI..."\n\
if /usr/local/bin/metta_knowledge_cli --help > /dev/null 2>&1; then\n\
    echo "   ✓ MeTTa Knowledge CLI is working"\n\
else\n\
    echo "   ✗ MeTTa Knowledge CLI failed"\n\
fi\n\
echo ""\n\
echo "3. Testing MeTTa REPL..."\n\
if /usr/local/bin/metta-repl --version > /dev/null 2>&1; then\n\
    echo "   ✓ MeTTa REPL is working"\n\
else\n\
    echo "   ✗ MeTTa REPL failed"\n\
fi\n\
echo ""\n\
echo "4. Checking Metta files..."\n\
if [ -d "/app/base" ] && [ -d "/app/example" ] && [ -d "/app/reason" ]; then\n\
    echo "   ✓ Metta files present"\n\
    echo "     - Base: $(ls -1 /app/base/*.metta 2>/dev/null | wc -l) files"\n\
    echo "     - Examples: $(find /app/example -name "*.metta" 2>/dev/null | wc -l) files"\n\
    echo "     - Reason: $(ls -1 /app/reason/*.metta 2>/dev/null | wc -l) files"\n\
else\n\
    echo "   ✗ Metta files missing"\n\
fi\n\
echo ""\n\
echo "All tests completed!"' \
> /usr/local/bin/test-installation && chmod +x /usr/local/bin/test-installation

# Default command - start with bash for interactive use
CMD ["/bin/bash"]