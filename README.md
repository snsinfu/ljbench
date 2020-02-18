# ljbench

[![Build Status][travis-badge]][travis-url]

Molecular dynamics simulation of Lennard-Jones fluid for benchmarking. The code
is _not_ manually optimized (i.e., no hand vectorization) so that the benchmark
program models quick simulation programs written by non-experts.

[travis-badge]: https://travis-ci.org/snsinfu/ljbench.svg?branch=master
[travis-url]: https://travis-ci.org/snsinfu/ljbench

## Usage

Use Makefile task `run` to run a benchmark simulation program. The simulation
program prints the consumed time per simulation step in milliseconds. Lower the
better.

```sh
$ make run OPTFLAGS="-O2"
...
1.238 ms/step
```

Change flags and iterate. Be sure to run `make clean` if you change optimization
flags. You may want to use `make clean run`.

```sh
$ make clean run OPTFLAGS="-O2 -march=native -mtune=native"
...
1.181 ms/step

$ make clean run OPTFLAGS="-O2 -march=native -mtune=native -ffast-math"
...
1.041 ms/step

$ make clean run OPTFLAGS="-O2 -march=native -mtune=native -mno-avx"
...
1.223 ms/step
```

After an executable being built the benchmark can be run multiple times by just
repeating `make run`.

```sh
$ make run
1.164 ms/step
$ make run
1.167 ms/step
$ make run
1.194 ms/step
```

## Examples

The `examples` directory contains some experiments, including a comparison of
clang and gcc.
