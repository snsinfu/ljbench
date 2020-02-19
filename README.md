# ljbench

[![Build Status][travis-badge]][travis-url]

Molecular dynamics simulation of Lennard-Jones fluid for benchmarking. The code
is _not_ manually optimized (i.e., no hand vectorization) so that the benchmark
program models quick simulation programs written by non-experts.

[travis-badge]: https://travis-ci.org/snsinfu/ljbench.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/ljbench

## Usage

Use Makefile task `run` to run a benchmark simulation program. The simulation
program prints the throughput as a performance measure (higher the better).

```sh
$ make run OPTFLAGS="-O2"
...
807.754 step/s
```

Change flags and iterate. Be sure to run `make clean` if you change optimization
flags. You may want to use `make clean run`.

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

After an executable being built the benchmark can be run multiple times by just
repeating `make run`.

```sh
$ make run
859.106 step/s
$ make run
856.898 step/s
$ make run
837.521 step/s
```

## Examples

The `examples` directory contains some experiments, including a comparison of
clang and gcc.
