#!/bin/sh
ROOT=$(cd $(dirname $0); cd ..; pwd)

rm -rf ${ROOT}/build ${ROOT}/bin ${ROOT}/lib
