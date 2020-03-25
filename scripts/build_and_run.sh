#!/bin/sh

SCRIPT_DIR=$(dirname $0)

ROOT=$(cd ../$SCRIPT_DIR; pwd)

CONFIG="Debug"

set -ex

cmake --build $ROOT/build --config ${CONFIG}

bin/${CONFIG}/tracer.exe

set +x
