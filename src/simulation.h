#ifndef INCLUDED_SIMULATION_H
#define INCLUDED_SIMULATION_H

#include <stddef.h>
#include <stdint.h>


struct config
{
    size_t   particles;
    double   mass;
    double   epsilon;
    double   sigma;
    double   temperature;
    double   timestep;
    int64_t  steps;
    int64_t  log_interval;
    uint64_t seed;
};

struct result
{
    double energy;
};

extern struct config const default_config;


void run_simulation(struct config const *config, struct result *result);


#endif
