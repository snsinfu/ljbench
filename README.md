# ljbench

Molecular dynamics simulation of Lennard-Jones fluid for benchmarking. The code
is _not_ manually optimized (i.e., no hand vectorization) so that the benchmark
program models quick simulation programs written by non-experts.

## Usage

Use Makefile task `run` to run a benchmark simulation. It prints the consumed
time per simulation step in milliseconds. Lower the better.

```sh
$ make run OPTFLAGS="-O2"
...
1.238 ms/step
```

Change flags and iterate. Be sure to `make clean` to remove the old executable.

```sh
$ make clean run OPTFLAGS="-O2 -march=native -mtune=native"
1.181 ms/step
...
```

After an executable being built the benchmark can be run multiple times by
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
