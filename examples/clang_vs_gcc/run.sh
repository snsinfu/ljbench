#!/bin/sh

OPTFLAGS="-O2 -march=native -mtune=native"

: ${CLANG:=clang}
: ${GCC:=gcc}

cd "$(dirname "$0")/../.."

echo "# clang"
make clean all CC=${CLANG} OPTFLAGS="${OPTFLAGS}" > /dev/null
make run
make run
make run

echo "# gcc"
make clean all CC=${GCC} OPTFLAGS="${OPTFLAGS}" > /dev/null
make run
make run
make run
