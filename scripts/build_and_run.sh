#!/bin/sh

ROOT=$(cd $(dirname $0); cd ..; pwd)

CONFIG="Debug"

set -ex

cmake --build $ROOT/build --config ${CONFIG}

bin/${CONFIG}/tracer.exe

set +x
