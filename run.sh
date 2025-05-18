#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status

BUILD_TYPE=${1:-debug}
BUILD_TYPE=$(echo $BUILD_TYPE | tr '[:lower:]' '[:upper:]')

if [[ "$BUILD_TYPE" != "DEBUG" && "$BUILD_TYPE" != "RELEASE" ]]; then
    echo "Error: Build type must be either 'debug' or 'release'"
    exit 1
fi

cd build
BUILD_DIR="build_${BUILD_TYPE,,}"
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Only run cmake if CMakeCache.txt doesn't exist or if CMAKE_BUILD_TYPE has changed
if [ ! -f CMakeCache.txt ] || ! grep -q "CMAKE_BUILD_TYPE:STRING=$BUILD_TYPE" CMakeCache.txt; then
    echo "Running CMake configuration..."
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
fi

# Use parallel build with number of CPU cores
CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)
echo "Building with $CORES cores..."
make -j$CORES

# Run the application
./appnterm