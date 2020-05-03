#!/bin/sh

ROOT=$(cd $(dirname $0); cd ..; pwd)

CONFIG="Release"

set -ex

cmake --build "$ROOT/build" --config ${CONFIG}

bin/${CONFIG}/tracer.exe

set +x
