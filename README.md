# ljbench

[![Build Status][travis-badge]][travis-url]

Molecular dynamics simulation for CPU and compiler benchmarking. The code is
_not_ manually optimized (no hand-rolled vectorization) to model quick
simulation programs written by non-experts.

[travis-badge]: https://travis-ci.org/snsinfu/ljbench.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/ljbench

## Usage

Makefile target `run` builds the benchmark program if not exists and to runs a
numerical simulation. After completion it prints the throughput as a performance
measure (higher the better).

```sh
$ make run OPTFLAGS="-O2"
...
807.754 step/s
```

Change optimization flags and iterate using `make clean run`:

```sh
$ make clean run OPTFLAGS="-O2 -march=native -mtune=native"
...
846.74 step/s

$ make clean run OPTFLAGS="-O2 -march=native -mtune=native -ffast-math"
...
960.615 step/s

$ make clean run OPTFLAGS="-O2 -march=native -mtune=native -mno-avx"
...
817.661 step/s
```

Once an executable being built, the same benchmark can be run multiple times by
just repeating `make run`.

```sh
$ make run
859.106 step/s
$ make run
856.898 step/s
$ make run
837.521 step/s
```

## Examples

The `examples` directory contains some experiments. A comparison of clang and
gcc runs like the following (clang tends to generate faster code than gcc for
this kind of naive numerical simulation).

```
$ cd examples/clang_vs_gcc
$ ./run.sh
# clang
1190.64 step/s
1243.9 step/s
1243.32 step/s
# gcc
1018.49 step/s
1017.52 step/s
1054.61 step/s
```
