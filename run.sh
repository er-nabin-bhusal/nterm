#!/bin/bash

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
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ../..
make
./appnterm