#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include "simulation.h"


struct options
{
    int64_t   steps;
    size_t    particles;
    uint64_t  seed;
    clockid_t clock_id;
    int       verbose;
};

enum
{
    DEFAULT_STEPS         = 5000,
    DEFAULT_PARTICLES     = 500,
    VERBOSE_LOG_INTRERVAL = 1000,
};

static struct config cook_config(struct options const *options);
static void          parse_options(int argc, char **argv, struct options *options);
static long          must_parse_number(char const *arg);
static void          show_usage();
static double        timestamp(clockid_t clock_id);


int main(int argc, char **argv)
{
    struct options options = {
        .steps     = DEFAULT_STEPS,
        .particles = DEFAULT_PARTICLES,
        .seed      = 0,
        .clock_id  = CLOCK_PROCESS_CPUTIME_ID,
        .verbose   = 0,
    };
    parse_options(argc, argv, &options);

    struct config config = cook_config(&options);
    struct result result;
    double start_time = timestamp(options.clock_id);
    run_simulation(&config, &result);
    double end_time = timestamp(options.clock_id);

    double elapsed_time = end_time - start_time;
    double throughput = (double) options.steps / elapsed_time;
    printf("%g step/s\t%g energy\n", throughput, result.energy);

    return EXIT_SUCCESS;
}


static
struct config cook_config(struct options const *options)
{
    struct config config = default_config;

    config.particles = options->particles;
    config.steps = options->steps;

    if (options->verbose > 0) {
        config.log_interval = VERBOSE_LOG_INTRERVAL;
    }

    return config;
}


static
void parse_options(int argc, char **argv, struct options *options)
{
    for (int ch; (ch = getopt(argc, argv, "t:n:s:wvh")) != -1; ) {
        switch (ch) {
        case 't':
            options->steps = (int64_t) must_parse_number(optarg);
            break;

        case 'n':
            options->particles = (size_t) must_parse_number(optarg);
            break;

        case 's':
            options->seed = (uint64_t) must_parse_number(optarg);
            break;

        case 'w':
            options->clock_id = CLOCK_MONOTONIC;
            break;

        case 'v':
            options->verbose++;
            break;

        case 'h':
            show_usage();
            exit(EXIT_SUCCESS);
        }
    }
}


static
long must_parse_number(char const *arg)
{
    char *end;
    long num = strtol(arg, &end, 10);
    if (end == arg || *end != '\0') {
        fprintf(stderr, "invalid number: %s", arg);
        exit(EXIT_FAILURE);
    }
    return num;
}


static
void show_usage()
{
    static char const usage[] =
        "usage: ljbench [-wvh] [-t <steps>] [-n <parts>] [-s <seed>]\n"
        "\n"
        "Simulate Lennard-Jones fluid. Prints out mean elapsed time per step\n"
        "and final mean potential energy on exit.\n"
        "\n"
        "options:"
        "  -t <steps>  number of simulation steps (default: 1000)\n"
        "  -n <parts>  number of particles (default: 500)\n"
        "  -s <seed>   random seed\n"
        "  -w          measure wall time instead of cpu time\n"
        "  -v          enable verbose logging\n"
        "  -h          show this usage message and exit\n"
        "\n"
        "Larger <steps> and <parts> smooth out measurement variation.\n"
        "\n";
    fputs(usage, stderr);
}


static
double timestamp(clockid_t clock_id)
{
    struct timespec t;

    if (clock_gettime(clock_id, &t) == -1) {
        return NAN;
    }
    return (double) t.tv_sec + (double) t.tv_nsec / 1e9;
}
