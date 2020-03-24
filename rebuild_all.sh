#!/bin/sh

rm -rf build bin lib

ROOT=$(cd $(dirname $0); pwd)
CONFIGS="Debug Release"

set -ex

if [ $# -eq 1 ]; then
    CONFIGS=$1 
fi

for c in $CONFIGS; do
    cmake -H$ROOT -B$ROOT/build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=$c
    cmake --build $ROOT/build --config $c
done

set +x