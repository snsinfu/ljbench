#!/bin/sh

OPTFLAGS="-O2 -march=native -mtune=native"

cd "$(dirname "$0")/../.."

echo "# sse"
make clean all OPTFLAGS="${OPTFLAGS} -msse4 -mno-avx" > /dev/null
make run
make run
make run

echo "# avx"
make clean all OPTFLAGS="${OPTFLAGS} -mavx -mno-avx2" > /dev/null
make run
make run
make run

echo "# avx2"
make clean all OPTFLAGS="${OPTFLAGS} -mavx2" > /dev/null
make run
make run
make run
