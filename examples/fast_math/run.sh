#!/bin/sh

OPTFLAGS="-O2 -march=native -mtune=native"

cd "$(dirname "$0")/../.."

echo "# default"
make clean all OPTFLAGS="${OPTFLAGS}" > /dev/null
make run
make run
make run

echo "# -ffast-math"
make clean all OPTFLAGS="${OPTFLAGS} -ffast-math" > /dev/null
make run
make run
make run

echo "# -funsafe-math-optimizations"
make clean all OPTFLAGS="${OPTFLAGS} -funsafe-math-optimizations" > /dev/null
make run
make run
make run
