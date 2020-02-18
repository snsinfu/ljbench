#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include "simulation.h"


static void parse_options(int argc, char **argv, struct config *config);
static long parse_option_int(char const *arg);
static void show_usage();


int main(int argc, char **argv)
{
    struct config config = default_config;
    parse_options(argc, argv, &config);

    clock_t start_clock = clock();
    struct result result;
    run_simulation(&config, &result);
    clock_t end_clock = clock();

    double elapsed_time = (double) (end_clock - start_clock) / (double) CLOCKS_PER_SEC;
    double step_time = elapsed_time / (double) config.steps;
    printf("%g\t%g\n", step_time, result.mean_energy);

    return EXIT_SUCCESS;
}


static
void parse_options(int argc, char **argv, struct config *config)
{
    enum { VERBOSE_LOG_INTRERVAL = 10000 };

    for (int ch; (ch = getopt(argc, argv, "t:n:s:vh")) != -1; ) {
        switch (ch) {
        case 't':
            config->steps = (int64_t) parse_option_int(optarg);
            break;

        case 'n':
            config->particle_count = (size_t) parse_option_int(optarg);
            break;

        case 's':
            config->seed = (uint64_t) parse_option_int(optarg);
            break;

        case 'v':
            config->log_interval = VERBOSE_LOG_INTRERVAL;
            break;

        case 'h':
            show_usage();
            exit(EXIT_SUCCESS);
        }
    }
}


static
long parse_option_int(char const *arg)
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
        "usage: ljbench [-vh] [-t <steps>] [-n <parts>] [-s <seed>]\n"
        "\n"
        "Simulate Lennard-Jones fluid. Prints out mean elapsed time per step\n"
        "and final mean potential energy on exit.\n"
        "\n"
        "options:"
        "  -t <steps>  number of simulation steps (default: 1000)\n"
        "  -n <parts>  number of particles (default: 500)\n"
        "  -s <seed>   random seed\n"
        "  -v          enable verbose logging\n"
        "  -h          show this usage message and exit\n"
        "\n"
        "Larger <steps> and <parts> smooth out measurement variation.\n"
        "\n";
    fputs(usage, stderr);
}
