#!/bin/sh

SCRIPT_DIR=$(dirname $0)

ROOT=$(cd $(dirname $0); cd ..; pwd)

# clean folders if they are exist
rm -rf "${ROOT}/build" "${ROOT}/bin" "${ROOT}/lib"

mkdir "${ROOT}/build"
mkdir "${ROOT}/bin"
mkdir "${ROOT}/lib"

CONFIGS="Debug Release"

set -ex

if [ $# -eq 1 ]; then
    CONFIGS=$1 
fi

for c in $CONFIGS; do
    cmake -H"$ROOT" -B"$ROOT/build" -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=$c
    cmake --build "$ROOT/build" --config $c
done

set +x

ls -la bin/*/*
