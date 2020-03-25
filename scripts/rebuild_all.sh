#!/bin/sh

SCRIPT_DIR=$(dirname $0)

# call clean script
source "$SCRIPT_DIR/clean.sh"

ROOT=$(cd ../$SCRIPT_DIR; pwd)

CONFIGS="Debug Release"

set -ex

if [ $# -eq 1 ]; then
    CONFIGS=$1 
fi

for c in $CONFIGS; do
    cmake -H$ROOT -B$ROOT/build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=$c
    cmake --build $ROOT/build --config $c
done

ls -la bin/*/*

set +x